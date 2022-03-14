#include "dataIO.h"
using namespace std;

// load matrix from an ascii text file.
void load_matrix(istream* is, vector<double>* matrix)
{
    string line;
    string strnum;
    const string delim = " \t";
    // clear first
    matrix->clear();

    // parse line by line
    while (getline(*is, line))
    {
 //       matrix->push_back(vector<double>());

        for (string::const_iterator i = line.begin(); i != line.end(); ++i)
        {
            // If i is not a delim, then append it to strnum
            if (delim.find(*i) == string::npos)
            {
                strnum += *i;
                if (i + 1 != line.end()) { continue; }
                // If it's the last char, break
            }

            // If strnum still empty, previous char also delim. Ignore this char.
            if (strnum.empty()) { continue; }

            // If reach here, got a number. Convert to double.
            double number;

            istringstream(strnum) >> number;
            matrix->push_back(number);

            strnum.clear();
        }
    }
}

void load_vector(istream* is, vector<double>* vect)
{
    string line;
    string strnum;
    const string delim = " \t";
    // clear first
    vect->clear();

    // parse line by line
    getline(*is, line);
    for (string::const_iterator i = line.begin(); i != line.end(); ++i)
    {
        // If i is not a delim, then append it to strnum
        if (delim.find(*i) == string::npos)
        {
            strnum += *i;
            if (i + 1 != line.end()) { continue; }
            // If it's the last char, break
        }

        // If strnum still empty, previous char also delim. Ignore this char.
        if (strnum.empty()) { continue; }

        // If reach here, got a number. Convert to double.
        double number;

        istringstream(strnum) >> number;
        vect->push_back(number);

        strnum.clear();
    }

}

void print_matrix(vector<double> matrix, int n)
{
    cout << "The matrix is:" << endl;
    vector<double>::iterator it = matrix.begin();
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
            cout << *it++ << '\t';

        cout << endl;
    }

}
void print_vector(vector<double> vect)
{
    cout << "The vector is:" << endl;
    for (vector<double>::const_iterator it = vect.begin(); it != vect.end(); ++it)
        cout << *it << '\t';

    cout << endl;

}