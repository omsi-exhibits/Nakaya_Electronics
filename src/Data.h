#ifndef DATA_H
#define DATA_H
// number of sections
#define NUM_SECTIONS 4
class Data {
    public:
        Data();
        int count(int section);
        int changed(int section);
        void setCount(int section, int count);
    private:
        int sectionCount[NUM_SECTIONS];
        int sectionLastCount[NUM_SECTIONS];
        bool sectionChanged[NUM_SECTIONS];
};

#endif