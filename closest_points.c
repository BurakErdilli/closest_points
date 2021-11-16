#define MAX 500
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

struct Point {
    int x;
    int y;
};

struct Info {
    double distance;
    struct Point p1;
    struct Point p2;
};

struct Info min(struct Info x, struct Info y) {
    if (x.distance < y.distance)
        return x;
    else
        return y;
}

int compare_x_dist(const void *a, const void *b) {
    struct Point *p_a = (struct Point *) a;
    struct Point *p_b = (struct Point *) b;
    return (p_a->x - p_b->x);
}

int compare_y_dist(const void *a, const void *b) {
    struct Point *p_a = (struct Point *) a, *p_b = (struct Point *) b;
    return (p_a->y - p_b->y);
}

struct Info dist(struct Point p_a, struct Point p_b) {
    struct Info dInfo;
    dInfo.distance = sqrt(pow((p_a.x - p_b.x), 2) + pow((p_a.y - p_b.y), 2));
    dInfo.p1 = p_a;
    dInfo.p2 = p_b;
    return dInfo;
}



struct Info bruteForce(struct Point P[], int n) {
    struct Info dInfo;
    double min = DBL_MAX;
    int i, j;
    double tmpdist;
    for (i = 0; i < n; ++i)
        for (j = i + 1; j < n; ++j)
            tmpdist = dist(P[i], P[j]).distance;
    if (tmpdist < min) {
        min = tmpdist;
        dInfo.p1 = P[i];
        dInfo.p2 = P[j];
        dInfo.distance = min;
    }
    return dInfo;
}


struct Info collect_closest_points(struct Point narrow[], int size, double d) {
    struct Info dInfo;  
    int space = sizeof(struct Point);
    dInfo.distance = d;

    qsort(narrow, size, space, compare_y_dist);

    int i, j;
    for (i = 0; i < size; ++i) {
        for (j = i + 1; j < size && (narrow[j].y - narrow[i].y) < dInfo.distance; ++j) {
            if (dist(narrow[i], narrow[j]).distance < dInfo.distance) {
                dInfo = dist(narrow[i], narrow[j]);

            }
        }
    }

    return dInfo;
}


struct Info minimum_dist(struct Point P[], int n) {
    struct Info dInfo;
    if (2 <= n && n <= 3)
        return bruteForce(P, n);
    int middle = n / 2;
    struct Point midPoint = P[middle];

    struct Info distance_left = minimum_dist(P, middle);
    struct Info distance_right = minimum_dist(&P[middle], n - middle);

    struct Info d = min(distance_left, distance_right);

    struct Point narrow[n];
    int j = 0;
    int i;
    for (i = 0; i < n; i++)
        if (abs(midPoint.x - P[i].x) < d.distance) {
            narrow[j] = P[i];
            j++;
        }


    if (j < 2) {
        return d;
    }
    dInfo = min(d, collect_closest_points(narrow, j, d.distance));

    return dInfo;
}

struct Info closestPair(struct Point P[], int n) {
    int size = sizeof(struct Point);
    qsort(P, n, size, compare_x_dist);
    return minimum_dist(P, n);
}


int main() {
    struct Point A[MAX];
    struct Info m;
    char filename[25] = "sample.txt";
    FILE *fp = fopen(filename, "r");
    int i = 0;
    if (fp == NULL) {
        printf("unable to open the file");
        return -1;
    }
    printf("All the coordinates that are in file->> %s  \n\n",filename);
    while (fscanf(fp, "%d %d", &A[i].x, &A[i].y) == 2) {
        printf("Point %d ---> (x,y)  (%d , %d)  \n", i + 1, A[i].x, A[i].y);
        i++;
    }
    fclose(fp);



    m = closestPair(A, i);
    printf("\n\n\nClosest Distance: %.3f\n", m.distance);
    printf("Point 1: (%d, %d)\n", m.p1.x, m.p1.y);
    printf("Point 2: (%d, %d)\n", m.p2.x, m.p2.y);
    return 0;
}
