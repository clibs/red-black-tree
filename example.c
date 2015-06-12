// Example.

#include "src/jsw_rbtree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct centroid {
  double            mean;
  int               count;
} centroid_t;

typedef struct jsw_rbtree centroidset_t;

static int centroid_cmp(const void *p1, const void *p2)
{
	centroid_t *centroid1, *centroid2;

	centroid1 = (centroid_t*)p1;
	centroid2 = (centroid_t*)p2;

	if (centroid1->mean > centroid2->mean)
		return 1;

	else if (centroid1->mean < centroid2->mean)
		return -1;

	return 0;
}

static void *centroid_dup(void *p)
{
	void *dup_p;

	dup_p = calloc(1, sizeof(struct centroid));
	memmove(dup_p, p, sizeof(struct centroid));

	return dup_p;
}

static void centroid_rel(void *p)
{
	free(p);
}

centroidset_t *centroidset_new()
{
	jsw_rbtree_t *rbtree;
	rbtree = jsw_rbnew(centroid_cmp, centroid_dup, centroid_rel);

	return rbtree;
}

void centroidset_delete(centroidset_t *centroidset)
{
	jsw_rbdelete(centroidset);
}

int centroidset_weighted_insert(centroidset_t *centroidset, double mean, int weight)
{
	int ret;

	centroid_t *centroid;
	centroid = calloc(1, sizeof(centroid_t));

	centroid->mean = mean;
	centroid->count = weight;

	ret = jsw_rbinsert(centroidset, (void *)centroid);
	if (ret == 0) {
		printf("failed to insert the centroid with mean %f and weight %d\n", mean, weight);
		free(centroid);
		return -1;
	}

	return 0;
}

int centroidset_insert(centroidset_t *centroidset, double mean)
{
	return centroidset_weighted_insert(centroidset, mean, 1);
}

int centroidset_erase(centroidset_t *centroidset, double mean)
{
	int ret;
	centroid_t *centroid;

	centroid = calloc(1, sizeof(centroid_t));
  centroid->mean = mean;

	ret = jsw_rberase(centroidset, (void*)centroid);
	if (ret == 0) {
		printf("failed to erase the centroid with mean %f\n", mean);
		free(centroid);
		return -1;
	}

	return 0;
}

double centroidset_find(centroidset_t *centroidset, double mean)
{
	centroid_t *centroid, centroid_find;

	centroid_find.mean = mean;
	centroid = jsw_rbfind(centroidset, &centroid_find);
  if (!centroid) {
    return NAN;
  }
	return centroid->mean;
}

void centroidset_printset(centroidset_t *centroidset)
{
	centroid_t *centroid;

	jsw_rbtrav_t *rbtrav;
	rbtrav = jsw_rbtnew();

	centroid = jsw_rbtfirst(rbtrav, centroidset);
  printf("mean %f\n", centroid->mean);

	while ((centroid = jsw_rbtnext(rbtrav)) != NULL) {
		printf("mean %f\n", centroid->mean);
	}
}

int main()
{
	centroidset_t *centroidset;
	centroidset = centroidset_new();
	centroidset_insert(centroidset, 1.0);
	centroidset_insert(centroidset, 2.0);
	centroidset_insert(centroidset, 1.5);
	centroidset_insert(centroidset, 9.9);
	centroidset_insert(centroidset, 1.8);
	centroidset_insert(centroidset, 3.3);
	double ret;
	ret = centroidset_find(centroidset, 1.0f);
	printf("find 1.0: %f\n", ret);
	ret = centroidset_find(centroidset, 9.900000);
	printf("find 9.9: %f\n", ret);
	ret = centroidset_find(centroidset, 1.800000);
	printf("find 1.8: %f\n", ret);
	ret = centroidset_find(centroidset, 0);
	printf("find 0 (not there) %f\n", ret);
	ret = centroidset_erase(centroidset, 1.800000);
	printf("erase 1.800000: %f\n", ret);
	ret = centroidset_find(centroidset, 1.800000);
	printf("find 1.8: %f\n", ret);
	centroidset_printset(centroidset);
	centroidset_delete(centroidset);
}
