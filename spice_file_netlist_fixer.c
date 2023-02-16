/**
 * @file spice_file_netlist_fixer.c
 * @author Huang Rui (vowstar@gmail.com)
 * @brief Fix TSMCN28 SPI mosfet width
 * @version 0.1
 * @date 2023-02-08
 *
 */

#include <math.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    int result = 0;
    size_t match_count=0;
    size_t max_group=5;
    regex_t regex;
    regmatch_t regmatch[max_group];
    /*
     * TODO: M=>simM, LR=>lr, WR=>wr, UM=>U
     */
    result = regcomp(&regex, "(^M.+)W=([0-9\\.]+)U\\sL=([0-9\\.]+)U(\\s*$)", REG_EXTENDED);
    if (result) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    /* read each line in file */
    while ((read = getline(&line, &len, stdin)) != -1) {
        result = regexec(&regex, line, max_group, regmatch, 0);
        if (!result) {
            float w = 0.f;
            float l = 0.f;
            /* E.g.: Val: M2000 N2000 N200 VSS VSS nch_mac W=0.56U L=0.03U */
            char match_buf_f[regmatch[0].rm_eo - regmatch[0].rm_so + 1];
            /* E.g.: Val: M2000 N2000 N200 VSS VSS nch_mac */
            char match_buf_s[regmatch[1].rm_eo - regmatch[1].rm_so + 1];
            /* E.g.: 0.56 */
            char match_buf_w[regmatch[2].rm_eo - regmatch[2].rm_so + 1];
            /* E.g.: 0.03 */
            char match_buf_l[regmatch[3].rm_eo - regmatch[3].rm_so + 1];
            /* Clear strings */
            memset(match_buf_f, 0, sizeof(match_buf_f));
            memset(match_buf_s, 0, sizeof(match_buf_s));
            memset(match_buf_w, 0, sizeof(match_buf_w));
            memset(match_buf_l, 0, sizeof(match_buf_l));
            /* Copy strings */
            strncpy(match_buf_f, line + regmatch[0].rm_so, regmatch[0].rm_eo - regmatch[0].rm_so);
            strncpy(match_buf_s, line + regmatch[1].rm_so, regmatch[1].rm_eo - regmatch[1].rm_so);
            strncpy(match_buf_w, line + regmatch[2].rm_so, regmatch[2].rm_eo - regmatch[2].rm_so);
            strncpy(match_buf_l, line + regmatch[3].rm_so, regmatch[3].rm_eo - regmatch[3].rm_so);

            w = strtof(match_buf_w, NULL);
            l = strtof(match_buf_l, NULL);

            if (w < 2.7f) {
                /* Print lines */
                printf("%s", line);
            } else {
                bool find_solution = false;
                size_t m = 0;
                for (m = 2; m < 100; m++)
                {
                    char buf[100];
                    float t = w / m;
                    snprintf(buf, sizeof(buf), "%g", t);
                    if (strlen(buf) > 4 || t >= 2.7f) {
                        continue;
                    } else {
                        find_solution = true;
                        break;
                    }
                }
                if (find_solution) {
                    /* Print lines */
                    printf("%sW=%gU L=%gU M=%ld\n",match_buf_s, w / m, l, m);
                } else {
                    fprintf(stderr, "Error: %s", line);
                    fprintf(stderr, "Could not find solution\n");
                    exit(1);
                }
            }
        }
        else if (result == REG_NOMATCH) {
            /* Print lines */
            printf("%s", line);
        }
    }

    return 0;
}
