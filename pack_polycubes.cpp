//  USAGE
//      pack_polycubes cover_file rows_file
//
//  COVER FILE FORMAT
//      num_pairs   Number of 1s in cover matrix.
//      row col     Row and column indices of each 1 in matrix.
//      ...
//      num_opts    Number of optional columns in matrix
//                      (i.e., "at most one" vs. "exactly one" constraints).
//      col         Indices of optional columns.
//      ...
//
//  ROWS_FILE FORMAT
//      num_rows    Number of rows in cover matrix.
//      piece       Integer ID of piece represented by each row.
//      ...

#include "dlx.h" // https://github.com/possibly-wrong/dlx
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include <numeric>
#include <random>
using std::size_t;

size_t total_solutions = 0;
const size_t LIMIT = 10000;
size_t nodes = 0;

std::vector<int> pieces; // row in solution :-> piece ID
std::map<std::vector<int>, size_t> counts; // piece IDs :-> # solutions

std::ofstream out("puzzle_output.txt");



class Solver : public dlx::DLX<int, int>
{
public:
    virtual bool found()
{
    std::vector<int> ids;

    for (auto&& row : solution)
        ids.push_back(pieces[row]);

    std::sort(ids.begin(), ids.end());

    ++counts[ids];
    total_solutions++;

    if (total_solutions >= LIMIT)
        return false;

    return true;
}
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: pack_polycubes cover_file rows_file" << std::endl;
        return 0;
    }

    // Enter cover matrix.
    std::ifstream cover(argv[1]);
    Solver solver;
    // Enter number of (row, col) pairs.
    int n;
    cover >> n;
    // Enter pairs.
    for (int i = 0; i < n; ++i)
    {
        int row, col;
        cover >> row >> col;
        solver.add(row, col);
    }
    // Enter number of optional columns.
    cover >> n;
    // Enter optional columns.
    for (int i = 0; i < n; ++i)
    {
        int col;
        cover >> col;
        solver.optional(col);
    }

    // Enter rows of piece IDs.
    std::ifstream rows_file(argv[2]);
    rows_file >> n;
    for (int i = 0; i < n; ++i)
    {
        int piece;
        std::string s;
        rows_file >> piece >> s;
        pieces.push_back(piece);
    }

    solver.search();
    /*
    for (auto&& count : counts)
    {
        std::cout << count.second << " ";
        for (size_t i = 0; i < count.first.size(); ++i)
        {
            std::cout << count.first[i] << " ";
        }
        std::cout << std::endl;
    }
    */
    for (auto&& entry : counts)
{
    const std::vector<int>& pieces_set = entry.first;
    size_t sol_count = entry.second;

    // scrivi numero soluzioni
    out << sol_count << " ";

    // scrivi pezzi
    for (int p : pieces_set)
        out << p << " ";

        double diff = (sol_count > 0)
        ? (double)nodes / (double)sol_count
        : 0.0;

    out << "DIFF " << diff << "\n";

}
}