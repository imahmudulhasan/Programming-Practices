#include<stdio.h>
#include<string.h>

float convert_temp(float cel){
  float far = cel*(9.0/5.0) + 32;
    return far;

}

int main(){
    float far,cel;
    cel = convert_temp(0);
    printf("The Converted Temp is %f",cel);

    return 0;
}
