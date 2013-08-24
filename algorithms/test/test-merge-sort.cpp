#include <iostream>
/* *** */
#include <boost/assert.hpp>
#include <boost/assign.hpp>
/* *** */
#include <meave/algorithms/merge-sort.hpp>
#include <meave/commons.hpp>

using namespace boost::assign;

static void x001(void) {
    $::vector<int> arr;
    arr += 1000, 200, 30, 4;

    $::vector<int> brr(arr.size());

    meave::algorithms::merge_sort(arr.begin(), brr.begin(), arr.size(), [](const int x, const int y) -> bool { return x < y; });

    for ($::size_t i = 0; i < arr.size(); ++i) {
        $::cerr << i << ".\t" << arr[i] << $::endl;
    }
}

int
main(void) {
    x001();

    return 0;
}
