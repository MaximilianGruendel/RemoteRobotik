#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Rect getRectangle(int pI){

    //X-& Y-Koordinate, sowie Breite & Höhe der Rechteckausschnitte
    Rect tmp[] = {
                    //Rect(130, 850, 140, 150),
                    //Rect(300, 850, 150, 150),
                    Rect(250, 750, 250, 250),
                 };
    return tmp[pI];

}

int getAngle(){

    Mat image, gray, binary;

    /**
    // Bild mit Kamera aufnehmen
    //Mat image = getImageCam();

    VideoCapture cap(0);

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

    // Lade ein Bild aus Verzeichnis
    image = imread("./example_2.jpg");
    cvtColor(image, gray, COLOR_RGB2GRAY);


    // Wandle das Bild in ein Binärbild um
    threshold(gray, binary, 100, 255, THRESH_BINARY);

    // Array für die Ausschnitte
    Mat rec_image[2];

    for(int j = 0; j < 2; j++){

        // Ausschneiden der Rechtecke aus dem Ursprungsbild
        rec_image[j] = binary(getRectangle(j));

        // Finde Linien im Binärbild
        vector<Vec2f> lines;
        HoughLines(rec_image[j], lines, 20, CV_PI/180, 75);

        // Zeichne die Linien auf das Ursprungsbild
        Mat result = rec_image[j].clone();
        for(size_t i = 0; i < lines.size(); i++) {
            float rho = lines[i][0], theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a * rho, y0 = b * rho;
            pt1.x = cvRound(x0 + 1000 * (-b));
            pt1.y = cvRound(y0 + 1000 * (a));
            pt2.x = cvRound(x0 - 1000 * (-b));
            pt2.y = cvRound(y0 - 1000 * (a));
            line(result, pt1, pt2, Scalar(255, 0, 0), 1, LINE_AA);
        }

        // Berechne den Winkel jeder Linie
        for(size_t i = 0; i < lines.size(); i++) {
            float theta = lines[i][1];
            int angle = theta * 180 / CV_PI;
            cout << "Servo "<< j << "| Linie " << i << ": Winkel = " << angle << " Grad" << endl;
        }
        // Zeige das Ergebnisbild an
        imshow("Ausschnitt", rec_image[j]);
        imshow("Result", result);
        waitKey(0);

    }

    /**
    namedWindow("Ursprungsbild",CV_WINDOW_AUTOSIZE);
    namedWindow("Binär",CV_WINDOW_AUTOSIZE);
    imshow("Ursprungsbild", image);
    imshow("Binär", binary);
    waitKey(0);
    */

}

void detectPoints(){

    string pics[] = {   "./degree_90.jpeg",
                        "./degree_80.jpeg",
                        "./degree_70.jpeg",
                        "./degree_60.jpeg",
                        "./degree_50.jpeg",
                        "./degree_40.jpeg",
                        "./degree_30.jpeg",
                        "./degree_20.jpeg",
                        "./degree_10.jpeg",
                        "./degree_0.jpeg",
                        "./degree_-10.jpeg",
                        "./degree_-20.jpeg",
                        "./degree_-30.jpeg",
                        "./degree_-40.jpeg",
                        "./degree_-50.jpeg",
                        "./degree_-60.jpeg",
                        "./degree_-70.jpeg",
                        "./degree_-80.jpeg",
                        "./degree_-90.jpeg",
                        };

    for(int i = 0; i < 19; i++){

    Mat image = imread(pics[i]);


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

    Mat binary;

    //threshold(sub(Rect(250, 750, 250, 250)), binary, 50, 255, THRESH_BINARY);
    threshold(sub, binary, 15, 255, THRESH_BINARY);


    bool row_detect, col_detect;

    int detect1 = 0;
    int detect2 = 0;


    Point pt1;
    Point pt2;

    int mat_cols1 = binary.cols;
    int* arr_row1 = new int[mat_cols1];
    int row_index1 = 110;
    Mat row1 = binary.row(row_index1);
    for(int j = 0; j < row1.cols; j++){
        arr_row1[j] = static_cast<int>(row1.at<uchar>(j));
        //cout << arr_row1[j] << endl;
        if(arr_row1[j] > 0){
            row_detect = true;
            pt1.x = j;
            pt1.y = row_index1;
            break;
        }
    }

    if(!row_detect){
    //cout << "-----------------------------------------------" << endl;

    int mat_rows1 = binary.rows;
    int* arr_col1 = new int[mat_rows1];
    int col_index1 = 260;
    Mat col1 = binary.col(col_index1);
    for(int j = 0; j < col1.rows; j++){
        arr_col1[j] = static_cast<int>(col1.at<uchar>(j));
        //cout << arr_col1[j] << endl;
        if(arr_col1[j] > 0){
            col_detect = true;
            pt1.y = j;
            pt1.x = col_index1;
            break;
        }
    }
    }

    if(row_detect){
    //cout << "-----------------------------------------------" << endl;

    int mat_cols2 = binary.cols;
    int* arr_row2 = new int[mat_cols2];
    int row_index2 = 250;
    Mat row2 = binary.row(row_index2);
    for(int j = 0; j < row2.cols; j++){
        arr_row2[j] = static_cast<int>(row2.at<uchar>(j));
        //cout << arr_row2[j] << endl;
        if(arr_row2[j] > 0){
            pt2.x = j;
            pt2.y = row_index2;
            break;
        }
    }
    }

    if(col_detect){
    //cout << "-----------------------------------------------" << endl;

    int mat_rows2 = binary.rows;
    int* arr_col2 = new int[mat_rows2];
    int col_index2 = 400;
    Mat col2 = binary.col(col_index2);
    for(int j = 0; j < col2.rows; j++){
        arr_col2[j] = static_cast<int>(col2.at<uchar>(j));
        //cout << arr_col2[j] << endl;
        if(arr_col2[j] > 0){
            pt2.y = j;
            pt2.x = col_index2;
            break;
        }
    }
    }
    cout << "Punkt 1: " << pt1.x << " | " << pt1.y << endl;
    cout << "Punkt 2: " << pt2.x << " | " << pt2.y << endl;


    int length = pt2.x - pt1.x;

    double m = atan2(pt2.y - pt1.y, pt2.x - pt1.x);
    // Umrechnung in Grad
    double angle = m * 180 / CV_PI;

    cout << angle << endl;

    Mat result;
    cvtColor(binary, result, COLOR_GRAY2BGR);

    row_detect = false;
    col_detect = false;

    line(result, pt1, pt2, Scalar(255, 0, 0), 1, LINE_AA);

    imshow("Binary", result);


    waitKey(0);

    }
}

int main(int argc, char** argv) {

    //return getAngle();
    detectPoints();

    return 0;

}

