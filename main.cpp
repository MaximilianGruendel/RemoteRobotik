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

int main(int argc, char** argv) {

    return getAngle();

}

