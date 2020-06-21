#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include "Geometry.h"

using namespace std;

double det(const Vertex &u, const Vertex &v) { // cross
    return u.x * v.y - u.y * v.x;
}

double dist(Vertex const &u, Vertex const &v) {
    return sqrt(pow((v.x - u.x), 2) + pow((v.y - u.y), 2));
}

bool pointInTriangle(Vertex a, Vertex b, Vertex c, Vertex p) {
    // check if point p is within the triangle abc: the point must be at the left
    // of each edge
    Vertex ab = b - a;
    Vertex bc = c - b;
    Vertex ca = a - c;

    if (det(ab, p - a) > 0 && det(bc, p - b) > 0 && det(ca, p - c) > 0)
        return true;
    else
        return false;
}

int orient(const Vertex &p, const Vertex &q, const Vertex &r) {
    // counter clock wise: +ve: upward direction: +ve orient
    long x = ((q.x*r.y-r.x*q.y) - p.x*(r.y-q.y) + p.y*(r.x-q.x));
    if (x == 0) return 0;
    else return (x>0) ? 1 : -1 ;
}

void log(string s) {
    cout << s << "\n";
}

vector<Vertex> readvertices(int argc, char *argv[]) {
    string current_exec_name = argv[0]; // Name of the current exec program
    vector<string> all_args;
    vector<Vertex> vertices;
    ifstream myFile;
    stringstream ss;
    string line;
    long a, b;
    if (argc > 1) { // convert args to string from char*
        all_args.assign(argv + 1, argv + argc);
    }

    if (argc > 1 && all_args[0] == "help") { // asking help
        cout << "Polygon Triangulation Program.\n"
            << "Input: file with space seperated vertices as command line argument.\n"
            << "Output: each line consisting of triangle vertices\n"
            << "\nFile format: first line n = number of vertices\n"
            << "next n lines each having space seperated integer coordinates of polygon vertices listed in anti-clockwise order on polygon\n";
        exit(0);
    }
    else if (argc == 1) { // no args
        cout << "Manual input: File not specified in args.\n";
        cout << "Enter no of vertices: ";
        long n; cin >> n;
        cout << "Enter vertices as space seperated coordinates in each line\n";
        for (long i=0;i<n;i++) {
            cin >> a >> b;
            vertices.emplace_back(a, b);
        }
    }
    else if (argc != 2) { // incorrect args
        cout << "Only one file name is taken as argument.\n"
            << "Type 'help' as arg to know more." << endl;
        exit(0);
    }
    else { // file operation
      myFile.open(all_args[0]);
      cout << "Opening: " << all_args[0] << "\n";
      if (myFile.is_open()) {
        long long n;
        getline(myFile, line);
        ss << line;
        ss >> n;
        for (long long i = 0; i < n; i++) {
          getline(myFile, line);
          ss.clear();
          ss << line;
          ss >> a >> b;
          vertices.emplace_back(a, b);
        }
      } else {
        cout << "Error opening file. please check and try again." << endl;
        exit(0);
      }
      myFile.close();
    }
    return vertices;
}

#endif
