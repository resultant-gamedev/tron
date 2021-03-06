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

#ifndef SEARCH_HUMAN_H
#define SEARCH_HUMAN_H

#include "vectors_maths/maths_vect.h"
#include "win_size.h"
#include "root/root.h"
#include <opencv2/opencv.hpp>
#include <vector>

class SearchHuman {
public:
    SearchHuman(std::vector< cv::Mat > &, cv::Mat &, int &, int &, int &);
    SearchHuman(cv::Mat &, cv::Mat &, int &, int &, int &);
    ~SearchHuman();
    void first_search();
    void search();
    void unification();
    void clear_partition();
    int at(int const &p);
    void replace(int const &a, int const &b);

private:

    void search_human(const Vect<int> &v);
    std::vector<int> *partition;
    std::vector<int> id_area;
    bool out_zone(Vect<int> const &);
    void new_id(Vect<int> const &, int &);
    void search_partitions();
    int scan_pixel(Vect<int> v);
    void new_value(Vect<int> const &, Vect<int> const &, int &);
    bool fusion(Vect<int> const&);
    int id_non_null(Vect<int> const&);

    int green_color, blue_color, red_color;
    Vect<long int> centroid;
    long int surface;
    cv::Mat mat_frame;
    std::vector< cv::Mat > vect_imgs;
    cv::Mat img;
    std::vector<int>::iterator max;
    long int id_img;
    long int s;
};

// retoune l'identifiant de la partition d'un point, même si le point est en dehors de l'image
// return the area id from the point, even if the point is out from the picture
inline int SearchHuman::id_non_null(Vect<int> const& a) {

    if(is_null(a))
        return -1;

    return at(coord_gray(a));

}

#endif // SEARCH_HUMAN_H
