
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>

class TachyonVal {
public:
    double num;
    std::map<std::string, TachyonVal>* props;
    void* other_data;
    TachyonVal(double num) {
        this->num = num;
    }
    TachyonVal(std::map<std::string, TachyonVal>* props) {
        this->props = props;
    }
    TachyonVal(std::map<std::string, TachyonVal>* props, void* other_data) {
        this->props = props;
        this->other_data = other_data;
    }
    // ~TachyonVal() {
    //     free(props);
    //     free(other_data);
    // }
};

TachyonVal String = TachyonVal(new std::map<std::string, TachyonVal>({}));
TachyonVal Vector = TachyonVal(new std::map<std::string, TachyonVal>({}));
TachyonVal Function = TachyonVal(new std::map<std::string, TachyonVal>({}));
TachyonVal print = TachyonVal(new std::map<std::string, TachyonVal>({}), new std::function<TachyonVal(const std::vector<TachyonVal>&)>([](const std::vector<TachyonVal>& args) {
    std::cout << args.at(0).num << '\n';
    return TachyonVal(0.0);
}));

    int main(){
        auto start = std::chrono::system_clock::now();

TachyonVal i=TachyonVal(0.0);while((TachyonVal((i).num<(TachyonVal(10000000.0)).num)).num != 0.0) {i=TachyonVal((i).num+(TachyonVal(1.0)).num);
{
TachyonVal x=TachyonVal((TachyonVal(2.0)).num+(TachyonVal(2.0)).num);
}}

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";


return 0;
}