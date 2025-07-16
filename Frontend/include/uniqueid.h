#include <cstdlib>
#include <ctime>
#include <climits>

//generate random unique IDs.

class RandomIdGenerator {
public:
    static void init() 
    {
        std::srand(std::time(nullptr));
    }

    static int generate() {
        int r = (std::rand() << 16) | (std::rand() & 0xFFFF);
        if (r == INT_MIN) r += 1;
        return std::abs(r);
    }
};
