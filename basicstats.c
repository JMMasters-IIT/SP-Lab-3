#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Jack Miller 11.22.2024
//This program calculates the count, min, max, average/mean, median, standard deviation, and sum
//of a file that's put into it. 

//function prototypes
float* read_data_from_file(const char* filename, size_t* size, size_t* capacity);
double get_min(const float* data, size_t size);
double get_max(const float* data, size_t size);
double get_mean(const float* data, size_t size);
double get_sum(const float* data, size_t size);
double get_median(float* data, size_t size);
double get_stddev(const float* data, size_t size);
int compare_floats(const void* a, const void* b);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    size_t size = 0, capacity = 0;

    float* data = read_data_from_file(filename, &size, &capacity);
    if (!data) {
        fprintf(stderr, "Failed to read data from file.\n"); //Error if unable to read from file
        return 1;
    }

    double min = get_min(data, size);
    double max = get_max(data, size);
    double sum = get_sum(data, size);
    double mean = get_mean(data, size);
    double median = get_median(data, size);
    double stddev = get_stddev(data, size);

    printf("Results:\n");
    printf("--------\n");
    printf("Num values:      %zu\n", size);
    printf("       Min:      %.4f\n", min);
    printf("       Max:      %.4f\n", max);
    printf("      Mean:      %.4f\n", mean);
    printf("    Median:      %.4f\n", median);
    printf("    StdDev:      %.4f\n", stddev);
    printf("       Sum:      %.4f\n", sum);
    printf("Unused array capacity: %zu\n", capacity - size);

    free(data); //Free up memory
    return 0;
}

//Reads data from a file and puts it into an array
float* read_data_from_file(const char* filename, size_t* size, size_t* capacity) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    size_t initial_capacity = 20; //Starting capacity of array
    float* data = malloc(initial_capacity * sizeof(float));
    if (!data) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    *size = 0;
    *capacity = initial_capacity;

    float value;
    while (fscanf(file, "%f", &value) == 1) {
        if (*size == *capacity) { //Check if we need to make the array bigger
            *capacity *= 2;
            float* new_data = realloc(data, *capacity * sizeof(float));
            if (!new_data) {
                perror("Error reallocating memory");
                free(data); //Free up memory
                fclose(file);
                return NULL;
            }
            data = new_data;
        }
        data[*size] = value;
        (*size)++;
    }
    fclose(file);
    return data;
}

double get_min(const float* data, size_t size) { //Find the minimum value
    double min = data[0];
    for (size_t i = 1; i < size; i++) {
        if (data[i] < min) min = data[i];
    }
    return min;
}

double get_max(const float* data, size_t size) { //Find the maximum value
    double max = data[0];
    for (size_t i = 1; i < size; i++) {
        if (data[i] > max) max = data[i];
    }
    return max;
}

double get_sum(const float* data, size_t size) { //Sum all the numbers
    double sum = 0.0;
    for (size_t i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum;
}

double get_mean(const float* data, size_t size) { //Find the average/mean of the data
    double sum = get_sum(data, size); //Use sum() to help here
    return sum / size;
}

double get_median(float* data, size_t size) { //Find the median value
    qsort(data, size, sizeof(float), compare_floats); //Sort data

    if (size % 2 == 0) { //If there's an even number of data, then add the middle values and divid by 2 for the answer
        return (data[size / 2 - 1] + data[size / 2]) / 2.0;
    } 
    else { // If odd number of data, then return middle value
        return data[size / 2];
    }
}

double get_stddev(const float* data, size_t size) { //Calculate the standard deviation
    double mean_value = get_mean(data, size); //Call mean/avg function
    double variance = 0.0;
    for (size_t i = 0; i < size; i++) {
        variance += (data[i] - mean_value) * (data[i] - mean_value);
    }
    variance = variance/size;
    return sqrt(variance);
}

int compare_floats(const void* a, const void* b) { //Used in Median to compare values
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
}