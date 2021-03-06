/*************************************************************************/
/* This file is part of Tron.                                            */
/*                                                                       */
/*  Tron is free software: you can redistribute it and/or modify         */
/*  it under the terms of the GNU General Public License as published by */
/*  the Free Software Foundation, either version 3 of the License, or    */
/*  (at your option) any later version.                                  */
/*                                                                       */
/*  Tron is distributed in the hope that it will be useful,              */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/*  GNU General Public License for more details.                         */
/*                                                                       */
/*  You should have received a copy of the GNU General Public License    */
/*  along with Tron.  If not, see <http://www.gnu.org/licenses/>.        */
/*************************************************************************/

#include "io_frames.h"
#include <stdio.h>

using namespace std;

IO_frames::IO_frames() : video_loaded(false) {
}

bool IO_frames::loaded() {
    return video_loaded;
}

bool IO_frames::make_vector(int const &nbr_imgs, cv::VideoCapture &capture) {
    // for all images
    // pour toutes les images
    for(int i = 0; i < nbr_imgs; ++i) {

        // capture the next image
        // capture l'image suivante

        cv::Mat buffer_img;
        capture >> buffer_img;

        // clone the image
        mat_frame = buffer_img.clone();

        // turn colors from RGB to BGR
        // change les couleurs de RVB vers BVR
        cv::cvtColor(mat_frame, mat_frame, CV_RGB2BGR);

        // free the memory
        // libère la mémoire
        buffer_img.release();

        // we put the new image in the vector
        // nous mettons l'image dans le vecteur
        vect_imgs.push_back(mat_frame);
    }
    return true;
}

// return nbr_imgs
// retourne le nombre d'images
int IO_frames::get_nbr_imgs() {
    return nbr_imgs;
}

// open only the video file and get the number of frames
// ouvre seulement la video et capture le nombre d'images
cv::VideoCapture IO_frames::open_input(const QString &fileName) {

    // read AVI video
    // capture la video à partir du fichier
    cv::VideoCapture capturevideo(fileName.toStdString().c_str());

    try {

        if (!capturevideo.isOpened())
            throw "(io_frames) error while opening file AVI.";

        // capture number of images from the video file
        // capture le nombre d'images dans le fichier video
        nbr_imgs = (int) capturevideo.get(CV_CAP_PROP_FRAME_COUNT);

        video_loaded = true;
    }
    catch (const char& strException)
    {
        cerr << "Exception caught !!" << endl;
        cerr << strException << endl;
        throw;
    }

    return capturevideo;
}

void IO_frames::load(const QString &fileName) {

    try {
        // open the video file
        // ouvre le fichier video
        cv::VideoCapture capturevideo = open_input(fileName);

        // reset vect_imgs
        vector<cv::Mat>().swap(vect_imgs);
        vect_imgs.resize(0);

        if(!make_vector(nbr_imgs, capturevideo))
            throw "(io_frames) error, all images are not loaded.";

        // we finished with the video, release
        // on en a fini avec la video, libération
        capturevideo.release();
    }
    catch (const char& strException)
    {
        cerr << "Exception caught !!" << endl;
        cerr << strException << endl;
        throw;
    }
}

// create an image from data and put this in the vector
// crée l'image à partir du data pour la mettre dans le vecteur
void IO_frames::push_data(char* data) {

    // create the image with the data
    // crée l'image avec les données
    cv::Mat buffer_img(HEIGHT, WIDTH, CV_8UC3, data);

    // clone the image to put this inside the vector
    // clone l'image pour l'introduire dans le vecteur
    mat_frame = buffer_img.clone();
    vect_imgs.push_back(mat_frame);

    buffer_img.release();
}

void IO_frames::save(std::vector< cv::Mat> &vect_motion, QString &fileName) {

    try {
        if(vect_motion.empty())
            throw "(io_frames) Can't save the file, empty video.\n";

        // create the size
        CvSize size;
        size.width = WIDTH;
        size.height = HEIGHT;

        cv::VideoWriter outputvideo(fileName.toStdString().c_str(), CV_FOURCC('I','Y','U','V'), 20, size);

        if(!outputvideo.isOpened())
            throw "(io_frames) Can't write in the file.\n";

        for (std::vector<cv::Mat>::iterator i = vect_motion.begin() ; i != vect_motion.end(); ++i) {
            mat_frame = *i;

            // turn colors from BGR to RGB
            // change les couleurs de BVR vers RVB
            cv::cvtColor(mat_frame, mat_frame, CV_BGR2RGB);

            outputvideo << mat_frame;

            // turn colors from RGB to BGR
            // change les couleurs de RVB vers BVR
            cv::cvtColor(mat_frame, mat_frame, CV_RGB2BGR);
        }

        outputvideo.release();

    }
    catch (const char& strException)
    {
        cerr << "Exception caught !!" << endl;
        cerr << strException << endl;
        throw;
    }
}
