#include "Utils.hpp"


namespace utils{

    int leerConfigFile(std::string key1){
        std::ifstream is_file("file.cfg");
        std::string line;
        int value1 = 0;
        while( std::getline(is_file, line) )
        {
            std::istringstream is_line(line);
            std::string key;
            if( std::getline(is_line, key, '=') )
            {
                std::string value;
                if( std::getline(is_line, value) ) {
                    if(key1.compare(key) == 0){
                        value1 = std::stoi(value);
                        std::cout << "key: " << key << 
                        " value: " << value1 << std::endl;
                        break;
                    }
                }
            }
        }
        return value1;
    }

    int showText(Point pos, string text, Mat img){
            putText(img, text, pos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 0));
    }

    double calcular_distancia_horizontal(double distancia_v, double a_px){
        double f = (2 * distancia_v * sin(DEGTORAD(ALPHA/2))) / sin(DEGTORAD(BETA));
        double distancia_h =  a_px * f / SCREEN_SIZE_X;
        return distancia_h;
    }

    double calcular_distancia(double alpha, double b_px){

        double largo_b_cam = 2.6;
        double altura_eje_motor = 7.2;
        double omega = 41; //angulo de vision camara
        double theta = (180 - omega)/2;
        double mi = 90 + alpha - omega / 2;
        double a, c, d, e, f, g, i, x;
        double beta;
        double kappa;
        double b_cm, j;


        a = largo_b_cam * sin(DEGTORAD(alpha));
        c = largo_b_cam * sin(DEGTORAD(90-alpha));
        d = altura_eje_motor - c;
        e = (d * sin(DEGTORAD(alpha - omega/2)))
            /sin(DEGTORAD(90-alpha+omega/2));
        f = e / (sin(DEGTORAD(alpha - omega / 2)));
        j = f * sin(DEGTORAD(omega)) / sin(DEGTORAD(theta));
        b_cm = b_px * j / SCREEN_SIZE_Y;
        i = sqrt(b_cm * b_cm + f * f - 2 * b_cm * f * cos(DEGTORAD(theta)));
        beta = RADTODEG(asin(b_cm * sin(DEGTORAD(theta)) / i));
        kappa = 180 - mi - beta;
        g = f * sin(DEGTORAD(beta)) / sin(DEGTORAD(kappa));
        x = g + e + a;
        return x;
    }
    void printToCoordinates(int x, int y, string text)
    {
        printf("\033[%d;%dH%s\n", x, x, text.c_str());
    }
}
