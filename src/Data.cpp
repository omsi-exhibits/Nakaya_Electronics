#include "Data.h"
Data::Data() {
    for(int i = 0; i < NUM_SECTIONS; i++) {
        sectionCount[i] = 0;
        sectionLastCount[i] = 0;
        sectionChanged[i] = false;
    }

}
int Data::count(int s) {
    return sectionCount[s];
}
int Data::changed(int s) {
    return sectionChanged[s];
}
void Data::setCount(int s, int count) {
    sectionLastCount[s] = sectionCount[s];
    // set sectionChanged to true if there is a change
    sectionLastCount[s] != sectionCount[s] ? sectionChanged[s] == true : sectionChanged[s] == false;
}