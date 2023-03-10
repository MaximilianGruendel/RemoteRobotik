#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

    // Lade das Bild
    Mat image = imread("/home/maximilian/10_WORKSPACE/01_Github/example_1.jpg", IMREAD_GRAYSCALE);

    // Überprüfen Sie, ob das Bild geladen wurde
    if (image.empty()) {
        cout << "Konnte das Bild nicht laden." << endl;
        return -1;
    }

    // Wandle das Bild in ein Binärbild um
    Mat binary;
    threshold(image, binary, 128, 255, THRESH_BINARY);

    // Finde Linien im Binärbild
    vector<Vec2f> lines;
    HoughLines(binary, lines, 1, CV_PI/180, 100);

    // Zeichne die Linien auf das Ursprungsbild
    Mat result = image.clone();
    for (size_t i = 0; i < lines.size(); i++) {
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
    for (size_t i = 0; i < lines.size(); i++) {
        float theta = lines[i][1];
        int angle = theta * 180 / CV_PI;
        cout << "Linie " << i << ": Winkel = " << angle << " Grad" << endl;
    }

    // Zeige das Ergebnisbild an
    imshow("Result", result);
    waitKey(0);

    return 0;
}
