//Utility functions for input validation

namespace utils {
    bool is_valid_num(int num) {
        if (num < 100000000 || num > 999999999) {
            return false;
        }
        return true;
    }
}