#include <cstdlib>
#include <ctime>
#include <climits>

class RandomIdGenerator {
public:
    // 初始化随机数种子（程序启动时调用一次）
    static void init() {
        std::srand(std::time(nullptr));
    }

    // 生成随机正整数ID（范围：1 ~ INT_MAX）
    static int generate() {
        int r = (std::rand() << 16) | (std::rand() & 0xFFFF);
        // 处理INT_MIN（-2147483648）取绝对值溢出的问题
        if (r == INT_MIN) r += 1;
        return std::abs(r);
    }
};
