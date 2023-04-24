#include "../inc/errors.h"
#include "../inc/strunct.h"
#include "../inc/actions.h"
#include "../inc/examination.h"
#include "../inc/time.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>


int main()
{
    int rc_s;
    setbuf(stdout, NULL);
    char str[MAX_LEN];
    struct stack *ps = NULL;
    printf("Введите выражение:\n");
    fgets(str, sizeof(str), stdin);
    rc_s = exam_for_stack(str, ps);
    full_free(&ps);
    if (rc_s == OK)
        printf("Correct!\n");
    else if (rc_s == NOT_OK)
        printf("Incorrect!\n");
    // full_free(&ps);
    return 0;
}

// {[(((((([{}{[]}])){{}}))){{}})]}{{{}}((([[[[]]]]))[][][]{{{[]}}})}