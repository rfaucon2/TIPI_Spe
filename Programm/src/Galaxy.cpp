#include "Galaxy.h"

Galaxy::Galaxy(){
    this->stars = new int[this->star_count];
}
Galaxy::~Galaxy(){
    delete this->stars;
}

void Galaxy::Update(){

}
void Galaxy::Draw(){
    
}
