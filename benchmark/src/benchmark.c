#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "list.h"


#define COMMON_STR_LEN 256

struct benchmark_ctx {
	int test_count;
	int log_flag;
	struct list_head test_item_list;
} t_benchmark_ctx;

typedef void (*item_init_func) (void);
typedef void (*item_run_func) (void);
typedef void (*item_finish_func) (void);

struct item_ops {
	item_init_func init;
	item_run_func run;		/* the function which do the benchmark */
	item_finish_func finish;
};

struct test_item {
	long long *time_list;		/* original test time */
	long long result_time;		/* test time after calculate */
	int score;
	int test_count;
	struct list_head entry;
	struct item_ops ops;
	char name[COMMON_STR_LEN];
};


inline long long get_time_us(void)
{
	struct timeval t = {};

	gettimeofday(&t, NULL);
	return t.tv_sec * 1000000 + t.tv_usec;
}

/* Now just use the simplest sort */
void __sort(long long *list, int count)
{
	int i, j;
	int flag = 0;

	for (i = 0; i < count; i++) {
		for (j = 0; j < count - i -1; j ++) {
			if (list[j] > list[j + 1]) {
				/* swap j and j+1 */
				list[j] ^= list[j + 1];
				list[j + 1] ^= list[j];
				list[j] ^= list[j + 1];
				flag = 0;
			}
			else {
				flag = 1;
			}
		}
		if (flag) {
			break;
		}
	}
}

static int calculate_average_result_time(struct test_item *ptr)
{
	if (ptr) {
		__sort(ptr->time_list, ptr->test_count);
		ptr->result_time = ptr->time_list[ptr->test_count / 2];
		return 0;
	}
}

static void result_output(struct list_head *test_time_list)
{
	struct test_item *pos = NULL;

	printf("\n\n--------------------------------SoC Benchmark--------------------------------\n\n");
	printf("[name]\t\t\t\t\t\t\t[time]\n");
	list_for_each_entry (pos, test_time_list, entry) {
		printf("%-32s\t\t\t%lld us\n", pos->name, pos->result_time);
	}
	printf("\n\n--------------------------------SoC Benchmark--------------------------------\n\n");

}

static void do_test_items(void)
{
	struct test_item *pos = NULL;
	long long t_start, t_end;

	list_for_each_entry (pos, &t_benchmark_ctx.test_item_list, entry) {
		int i = 0;
		for (; i < pos->test_count; i++) {
			if (pos->ops.init != NULL)
				pos->ops.init();
			t_start = get_time_us();
			pos->ops.run();
			t_end = get_time_us();
			pos->time_list[i] = t_end - t_start;
			if (pos->ops.finish != NULL)
				pos->ops.finish();
		}
		calculate_average_result_time(pos);
	}
	result_output(&t_benchmark_ctx.test_item_list);
}


int __add_test_item(struct list_head *head, struct item_ops *ops_ptr, const char *name, int test_count)
{
	struct test_item *ptr = malloc(sizeof(struct test_item));

	if (ptr) {
		memset(ptr, 0, sizeof(struct test_item));
		ptr->time_list = malloc(sizeof (long long) * test_count);
		if (ptr->time_list) {
			memset(ptr->time_list, 0, sizeof(long long) * test_count);
			ptr->ops = *ops_ptr;
			ptr->test_count = test_count;
			strncpy(ptr->name, name, COMMON_STR_LEN);
			list_add(&ptr->entry, head);
		}
		else {
			printf("%s error: no memory to alloc time_list\n", __func__);
			return -1;
		}
	}
	else {
		printf("%s error: no memory to alloc test_item\n", __func__);
		return -1;
	}
	return 0;
}

int register_test_item(const char *name, struct item_ops *ops_ptr)
{
	if (ops_ptr) {
		return __add_test_item(&t_benchmark_ctx.test_item_list, ops_ptr,
				name, t_benchmark_ctx.test_count);
	}
	else {
		return -1;
	}
}

void benchmark_init(int test_count, int log_flag)
{
	memset(&t_benchmark_ctx, 0, sizeof(struct benchmark_ctx));
	t_benchmark_ctx.test_count = test_count;
	t_benchmark_ctx.log_flag = log_flag;
	INIT_LIST_HEAD(&t_benchmark_ctx.test_item_list);
}


/*--------- Example: --------- */
void print_1(void)
{
	printf("1");
}

struct item_ops print_1_ops = {
	.init = NULL,
	.run = print_1,		/* the function which do the benchmark */
	.finish = NULL,
};
/*--------- Example: --------- */

int main(int argc, char **argv)
{
	printf("Benchmark: hello Tristan\n");
	benchmark_init(10, 0);
	register_test_item("print_1", &print_1_ops);
	do_test_items();
	return 0;
}

