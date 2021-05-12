#ifndef DATA_H
#define DATA_H
/* This class is to store the number of magnets that each puckSensor reads.
*  There is a puckSensor for each section.
*/
// number of sections
#define NUM_SECTIONS 4
class Data {
    public:
        Data();
        int count(int section);
        bool changed(int section);
        int lastCount(int section);
        void setCount(int section, int count);
    private:
        int sectionCount[NUM_SECTIONS];
        int sectionLastCount[NUM_SECTIONS];
        bool sectionChanged[NUM_SECTIONS];
};

#endif