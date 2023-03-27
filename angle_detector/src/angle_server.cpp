#include <ros/ros.h>
#include <angle_detector/AngleDetector.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <std_msgs/String.h>

using namespace cv;
using namespace std;

/**
Umfasst die Rechteckausschnitte der Bildaufnahme

X-& Y-Koordinate, sowie Breite & Höhe
*/
Rect getRectangle(int pI){

    Rect tmp[] = {
                    Rect(880, 130, 100, 100),
                    Rect(880, 240, 100, 100),
                    Rect(640, 220, 110, 110),
                 };
    return tmp[pI];

}

/**
Bestimmt den Mittelpunkt des Rotors an der Linie anhand der Werte einer Zeile oder Spalte
*/
int getMedium(Mat columrow, int cr){

    int tmp = 0;
    int num = 0;

    if(cr == 1){
        for(int i = 0; i < columrow.cols; i++){
            if(static_cast<int>(columrow.at<uchar>(i)) > 0){
                num++;
                tmp = tmp + i;
            }
        }
    } else if(cr == 2){
        for(int i = 0; i < columrow.rows; i++){
            if(static_cast<int>(columrow.at<uchar>(i)) > 0){
                num++;
                tmp = tmp + i;
            }
        }
    }

    if(num > 0){
        return tmp / num;
    } else {
        return 0;
    }
}

Point getCenter(Mat binary){

    Moments m = moments(binary, true);

    //Berechnung des Schwerpunkts
    Point center(m.m10/m.m00, m.m01/m.m00);

    return center;

}

Point* getPoints(Mat binary){

    Point center = getCenter(binary);

    //Ausgabe des Schwerpunktes
    Mat c;
    cvtColor(binary, c, COLOR_GRAY2BGR);

    circle(c, center, 5, Scalar(0, 255, 0), -1);
    Point a, b;
    a.x = center.x - 25;
    a.y = center.x - 25;
    b.x = center.y + 25;
    b.y = center.y + 25;

    rectangle(c, a, b, Scalar(255, 0, 0), 2);

    int correction = 25;
    Point pt1, pt2;

    int col1 = center.x - correction;
    int col2 = center.x + correction;
    int row1 = center.y - correction;
    int row2 = center.y + correction;

    if(getMedium(binary.row(col1), 1) > 0){
        pt1.x = getMedium(binary.row(col1), 1);
        pt1.y = col1;
        pt2.x = getMedium(binary.row(col2), 1);
        pt2.y = col2;
    } else {
        pt1.x = getMedium(binary.col(row1), 2);
        pt1.y = row1;
        pt2.x = getMedium(binary.row(row2), 2);
        pt2.y = row2;
    }

    static Point points[2];

    points[0] = pt1;
    points[1] = pt2;

    return points;

}

string angle(){

    /**
    // Bild mit Kamera aufnehmen
    Mat image = getImageCam();

    VideoCapture cap(0);
    cap.set(3, 1280);
    cap.set(4, 720);

    if (!cap.isOpened()) {
        cout << "cannot open camera";
    }

    while (true) {
        cap >> image;
        if(waitKey(10) >= 0){
            break;
        }
    }

    // Überprüfen Sie, ob das Bild geladen wurde
    if (image.empty()) {
        cout << "Konnte das Bild nicht laden." << endl;
        return -1;
    }

    //Wandle das Bild in ein Graubild um
    cvtColor(image, gray, COLOR_BGR2GRAY);
    */


    Mat image = imread("./hellBeleuchtung.jpg");
    Mat channels[3];

    split(image, channels);

    Mat sub;
    subtract(channels[2], channels[1], sub);

    Mat complete_binary;
    threshold(sub, complete_binary, 35, 255, THRESH_BINARY);

    Mat binary;
    Point *points;

    string answer = "";

    for(int i = 0; i < 3; i++){
        binary = complete_binary(getRectangle(i));
        points = getPoints(binary);

        double m = atan2(points[1].y - points[0].y, points[1].x - points[0].x);
        // Umrechnung in Grad
        double angle = m * 180 / CV_PI;

        answer = answer + "Servo " + to_String(i) + ": " + to_string(angle) + "DEG\n";
    }

    return answer;
}

string angleOne(int number){

    Mat image = imread("./hellBeleuchtung.jpg");
    Mat channels[3];

    split(image, channels);

    Mat sub;
    subtract(channels[2], channels[1], sub);

    Mat complete_binary;
    threshold(sub, complete_binary, 35, 255, THRESH_BINARY);

    Mat binary;
    Point *points;

    string answer = "";

    binary = complete_binary(getRectangle(number));
    points = getPoints(binary);

    double m = atan2(points[1].y - points[0].y, points[1].x - points[0].x);
    // Umrechnung in Grad
    double angle = m * 180 / CV_PI;

    answer = "Servo " + to_String(number) + ": " + to_string(angle) + "DEG\n";

    return answer;
}

bool response(angle_detector::AngleDetector::Request  &req, angle_detector::AngleDetector::Response &res){

    if(req.inputMotor >= 0 && req.inputMotor < 25){
        res.outResponse = angleOne(req.inputMotor);
    } else if(req.inputMotor == 25){
        res.outResponse = angle();
    } else {
        res.outResponse = "unvaild value! Has to be between 0 and 25.";
    }

    ROS_INFO("request: x=%i", req.inputMotor);
    ROS_INFO("sending back response: [%s]", res.outResponse.c_str());
    return true;

}

int main(int argc, char **argv){

    ros::init(argc, argv, "angle_server");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("angle_detector", response);
    ROS_INFO("Ready to search for angle.");
    ros::spin();

    return 0;

}
