#ifndef _PGMER_HPP_
#define _PGMER_HPP_


#include <cstdlib>
#include "utils.h"
#include "matrix.hpp"

class Pgmer {
public:
    std::string name;
    int row;
    int col;
    double *data;

public:
    Pgmer(){}

    Pgmer(const std::string &name) : name(name) {}

    Pgmer(const std::string &name, int row, int col) : name(name), row(row), col(col) {}

    Pgmer(const std::string &name, int row, int col, double *data) : name(name), row(row), col(col), data(data) {}


    void WriteImg() {

        std::ofstream f(this->name);
        if(f.is_open()){
            f << "P2\n" << this->col << " " << this->row << "\n255\n";
            int k = 1;
            for (int i = 0; i < this->row; ++i) {
                for (int j = 0; j < this->col; ++j) {
                    if(k % 10){
                        f << this->data[i * this->col + j] << " ";
                    } else {
                        f << this->data[i * this->col + j] << "\n";
                    }
                    k++;
                }
            }
        } else {
            perror("open outfile err!");
        }
        f.close();
    }

    bool ReadImg(std::string file_name) {
        FILE *pgm;
        char line[512], intbuf[100], ch;
        int type, nc, nr, maxval, i, j, k, found;

        if ((pgm = fopen(file_name.c_str(), "r")) == NULL)
        {
            printf("IMGOPEN: Couldn't open '%s'\n", file_name.c_str());
            return false;
        }


        /*** Scan pnm type information, expecting P5 ***/
        fgets(line, 511, pgm);
        sscanf(line, "P%d", &type);
        if (type != 5 && type != 2)
        {
            printf("IMGOPEN: Only handles pgm files (type P5 or P2)\n");
            fclose(pgm);
            return false;
        }

        /*** Get dimensions of pgm ***/
        fgets(line, 511, pgm);
        sscanf(line, "%d %d", &nc, &nr);
        this->row = nr;
        this->col = nc;

        /*** Get maxval ***/
        fgets(line, 511, pgm);
        sscanf(line, "%d", &maxval);
        if (maxval > 255)
        {
            printf("IMGOPEN: Only handles pgm files of 8 bits or less\n");
            fclose(pgm);
            return false;
        }

        this->data = new double[(nr * nc * sizeof(int))];
        if (this->data == NULL)
        {
            printf("IMGOPEN: Couldn't allocate space for image data\n");
            fclose(pgm);
            return false;
        }

        if (type == 5)
        {
            for (i = 0; i < nr; i++) {
                for (j = 0; j < nc; j++) {
                    this->SetPix(i, j, fgetc(pgm));
                }
            }
        }
        else if (type == 2)
        {

            for (i = 0; i < nr; i++) {
                for (j = 0; j < nc; j++) {

                    k = 0;  found = 0;
                    while (!found) {
                        ch = (char) fgetc(pgm);
                        if (ch >= '0' && ch <= '9') {
                            intbuf[k] = ch;  k++;
                        } else {
                            if (k != 0) {
                                intbuf[k] = '\0';
                                found = 1;
                            }
                        }
                    }
                    this->SetPix(i, j, atof(intbuf));
                }
            }
        } else {
            printf("IMGOPEN: Fatal impossible error\n");
            fclose(pgm);
            return false;
        }
        this->name = file_name;

        fclose(pgm);
    }

    bool SetPix(int i, int j, double val) {
        if(i > this->row || j > this->col) return false;
        this->data[this->col * i + j] = val;
        return true;
    }

    double GetPix(int i, int j) {
        if(i > this->row || j > this->col) return -1;
        return this->data[this->col * i + j];
    }

    Matrix<double> *To2DMatrix() {
        return new Matrix<double>(this->row, this->col, this->data);
    }

    void Parser(char *data) {
        char * temp = data;
        int sum = 0;
        while(*temp != '\0'){
            if(*temp == '\n'){
                sum++;
                if(sum == 3){
                    temp++;
                    break;
                }
            }
            temp++;
        }
        std::string s(data, temp);
        std::vector<std::string> head = split(s, "\n");
        int width = 0, height = 0;
        if(head.size() > 3){
            std::vector<std::string> w_h = split(head[1], " ");
            if(w_h.size() == 2){
                width = atoi(w_h[0].c_str());
                height = atoi(w_h[1].c_str());
            }
        }

        if(width == 0 || height == 0){
            perror("parser err!");
            return;
        }

        std::string str(temp);
        replace_all(str, "\n", " ");
        std::vector<std::string> mat = split(str, " ");
        this->row = height;
        this->col = width;
        this->data = new double[height * width];
        auto it = mat.begin();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                this->SetPix(i, j, atoi((*(it++)).c_str()));
            }
        }
    }

};



#endif //_PGMER_HPP_