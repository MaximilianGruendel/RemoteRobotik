#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

/**
Umfasst die Rechteckausschnitte der Bildaufnahme

X-& Y-Koordinate, sowie Breite & Höhe
*/
Rect getRectangle(int pI){

    Rect tmp[] = {
                    Rect(820, 140, 110, 110),   //Servo 0
                    Rect(825, 250, 110, 110),   //Servo 1
                    Rect(825, 370, 110, 110),   //Servo 2
                    Rect(820, 490, 110, 110),   //Servo 3
                    Rect(700, 120, 130, 110),   //Servo 4
                    Rect(710, 245, 130, 110),   //Servo 5
                    Rect(710, 375, 130, 110),   //Servo 6
                    Rect(700, 500, 130, 110),   //Servo 7
                    Rect(580, 110, 130, 110),   //Servo 8
                    Rect(580, 240, 130, 110),   //Servo 9
                    Rect(580, 380, 130, 110),   //Servo 10
                    Rect(580, 510, 130, 110),   //Servo 11
                    Rect(445, 110, 130, 110),   //Servo 12
                    Rect(445, 240, 130, 110),   //Servo 13
                    Rect(445, 380, 130, 110),   //Servo 14
                    Rect(445, 510, 130, 110),   //Servo 15
                    Rect(320, 120, 130, 110),   //Servo 16
                    Rect(320, 245, 130, 110),   //Servo 17
                    Rect(320, 375, 130, 110),   //Servo 18
                    Rect(320, 500, 130, 110),   //Servo 19
                    Rect(220, 130, 110, 110),   //Servo 20
                    Rect(220, 250, 110, 110),   //Servo 21
                    Rect(220, 375, 110, 110),   //Servo 22
                    Rect(220, 490, 110, 110),   //Servo 23
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
    //cout << "Center: " << center.x << " | " << center.y << endl;

    /**
    //Ausgabe des Schwerpunktes
    Mat c;
    cvtColor(binary, c, COLOR_GRAY2BGR);

    circle(c, center, 5, Scalar(0, 255, 0), -1);

    Point a, b;
    a.x = center.x - correction;
    a.y = center.y - correction;
    b.x = center.x + correction;
    b.y = center.y + correction;

    rectangle(c, a, b, Scalar(255, 0, 0), 2);
    */

    int correction = 25;
    Point pt1, pt2;

    int col1 = center.y - correction;
    int col2 = center.y + correction;
    int row1 = center.x - correction;
    int row2 = center.x + correction;

    /**
    cout << "col1: " << col1 << endl;
    cout << "col2: " << col2 << endl;
    cout << "row1: " << row1 << endl;
    cout << "row2: " << row2 << endl;
    */


    if(getMedium(binary.row(col1), 1) > 0){
        pt1.x = getMedium(binary.row(col1), 1);
        pt1.y = col1;
        pt2.x = getMedium(binary.row(col2), 1);
        pt2.y = col2;
    } else {
        pt2.y = getMedium(binary.col(row1), 2);
        pt2.x = row1;
        pt1.y = getMedium(binary.col(row2), 2);
        pt1.x = row2;
    }

    /**
    circle(c, pt1, 5, Scalar(0, 255, 0), -1);
    circle(c, pt2, 5, Scalar(0, 255, 0), -1);

    imshow("Result", c);
    waitKey(0);

    cout << "---Punkt_1: " << pt1.x << " | " << pt1.y << endl;
    cout << "---Punkt_2: " << pt2.x << " | " << pt2.y << endl;
    */

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


    Mat image = imread("./test_2.jpg");
    Mat channels[3];

    split(image, channels);

    /**
    imshow("RED", channels[2]);
    imshow("GREEN", channels[1]);
    imshow("BLUE", channels[0]);
    */

    Mat sub;
    subtract(channels[2], channels[1], sub);

    /**
    imshow("RED - GREEN", sub);
    */

    Mat complete_binary;
    threshold(sub, complete_binary, 35, 255, THRESH_BINARY);

    Mat binary;
    Point *points;

    string answer = "";

    for(int i = 0; i < 24; i++){
        binary = complete_binary(getRectangle(i));
        /**
        imshow("Test", binary);
        waitKey(0);
        */

        points = getPoints(binary);

        /**
        cout << i << endl;
        cout << "Punkt 1: " << points[0].x << " | " << points[0].y << endl;
        cout << "Punkt 2: " << points[1].x << " | " << points[1].y << endl;
        */

        double m = atan2(points[1].y - points[0].y, points[1].x - points[0].x);
        // Umrechnung in Grad
        int angle = m * 180 / CV_PI;

        if(angle < 0){
            angle = angle + 180;
        }
        if(angle > 90){
            angle = angle - 180;
        }

        /**
        cout << angle << endl;

        Mat result;
        cvtColor(binary, result, COLOR_GRAY2BGR);
        line(result, points[0], points[1], Scalar(255, 0, 0), 1, LINE_AA);

        imshow("Binary", result);
        waitKey(0);
        */

        answer = answer + "Servo " + to_string(i) + ": " + to_string(angle) + " DEG\n";

    }

    return answer;
}

int main(int argc, char** argv) {

    string tmp = angle();

    cout << tmp << endl;

    return 0;

}

