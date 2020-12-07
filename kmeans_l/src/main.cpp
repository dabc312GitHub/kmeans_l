#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

#define square(x) ((x) * (x))
typedef vector<pair<float, float>> v_pdd;

struct point2d
{
public:
    float x, y;
    int cluster;
};

struct color
{
public:
    float r, g, b;
};

int k;
point2d *centro;
color *clustercolor;

point2d *setPoints;
int n;
int *nb, *nbc;

void start(v_pdd vs_pos, int k_val)
{
    int i;
    //    n=10000;
    n = vs_pos.size();
    //    n=10;
    setPoints = new point2d[n];

    k = k_val;
    centro = new point2d[k];
    clustercolor = new color[k];
    nb = new int[k];
    nbc = new int[k];

    for (int i = 0; i < n; i++)
    {
        //        setPoints[i].x=(float)rand()/(float)RAND_MAX;
        //        setPoints[i].y=(float)rand()/(float)RAND_MAX;
        setPoints[i].x = (vs_pos[i].first);
        setPoints[i].y = (vs_pos[i].second);
        setPoints[i].cluster = 0;
    }
    for (int i = 0; i < k; i++)
    {
        centro[i] = setPoints[rand() % n];
        clustercolor[i].r = (float)rand() / (float)RAND_MAX;
        clustercolor[i].g = (float)rand() / (float)RAND_MAX;
        clustercolor[i].b = (float)rand() / (float)RAND_MAX;
    }
}

float euclidean_dist(int i, int j)
{
    float sq1 = square(setPoints[i].x - centro[j].x);
    float sq2 = square(setPoints[i].y - centro[j].y);
    return sqrt(sq1 + sq2);
}

void KMeansAssign()
{
    double dist, distmin;
    for (int i = 0; i < n; i++)
    {
        int curr;
        distmin = 1.0e8;
        for (int j = 0; j < k; j++)
        {
            dist = euclidean_dist(i, j);
            if (dist < distmin)
            {
                distmin = dist;
                curr = j;
            }
        }
        setPoints[i].cluster = curr;
    }
}

void KMeansCluster()
{
    for (int i = 0; i < k; i++)
    {
        nb[i] = 0;
        centro[i].x = 0;
        centro[i].y = 0;
    }

    for (int i = 0; i < n; i++)
    {
        int cl = setPoints[i].cluster;
        centro[cl].x += setPoints[i].x;
        centro[cl].y += setPoints[i].y;
        nb[cl]++;
    }

    nbc[0] = 0;
    for (int j = 0; j < k; j++)
    {
        if (j > 0)
        {
            nbc[j] = nbc[j - 1] + nb[j];
        }
        if (nb[j] > 0)
        {
            centro[j].x /= nb[j];
            centro[j].y /= nb[j];
        }
        //        cout <<"Class "<<j<<" "<<nb[j]<<" "<<centroid[j].x<<" "<<centroid[j].y<<" ["<<nbc[j]<<"]"<<endl;
        nb[j] = 0;
    }
}

void printPoints()
{
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(5.0f);

    glBegin(GL_POINTS);

    for (int i = 0; i < n; i++)
    {
        int cl = setPoints[i].cluster;
        //        cout <<set[i].x<<" " <<set[i].y<<endl;
        glColor3f(clustercolor[cl].r, clustercolor[cl].g, clustercolor[cl].b);
        glVertex2f(setPoints[i].x * 100.0f, setPoints[i].y * 100.0f);
    }
    glEnd();

    // Centroids
    glPointSize(20.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < k; i++)
    {
        //        cout <<set[i].x<<" " <<set[i].y<<endl;
        glColor3f(clustercolor[i].r + 0.5f, clustercolor[i].g + 0.5f, clustercolor[i].b + 0.5f);
        glVertex2f(centro[i].x * 100.0f, centro[i].y * 100.0f);
    }
    glEnd();

    glFlush();
    glutSwapBuffers();
}

void readCSV(const string &filename, v_pdd &vs_pos)
{
    ifstream infile(filename.c_str());

    if (!infile.is_open())
    {
        cout << "Error: Failed to open file." << endl;
        return;
    }
    string str_row, str_col;

    double pos_x, pos_y;
    for (int i = 0; getline(infile, str_row); ++i)
    {
        stringstream ss(str_row);
        getline(ss, str_col, ',');
        pos_x = stof(str_col);
        getline(ss, str_col, ',');
        pos_y = stof(str_col);

        vs_pos.emplace_back(pos_x, pos_y);
    }
    infile.close();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition(800, 800);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("k-Means clustering. ");

    glutDisplayFunc(printPoints);
    v_pdd vs_pos;
    string filename_csv;
    int k_val;
    cout << "ingrese el nombre del archivo con su extension: ";
    cin >> filename_csv;
    cout << "ingrese el k cluster: ";
    cin >> k_val;

    readCSV(filename_csv, vs_pos);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //    float bottomX = -1.0f;
    //    glTranslatef( bottomX, 1.0f , -1.0f );

    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    start(vs_pos, k_val);
    KMeansAssign();

    glutMainLoop();

    return 0;
}
