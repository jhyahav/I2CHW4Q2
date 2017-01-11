void MergeSuffixLists(char *list_1[], int len_1, char *list_2[], int len_2,
                       char *merged_list[]) {
    int ia, ib, ic, comp_return_val;
    ia = ib = ic = 0;
    for (ia = ib = ic = 0; ia < len_1 && ib < len_2; ic++) {
        comp_return_val = WordCompare(*(list_1 + ia), *(list_2 + ib));
        if (comp_return_val == -1 || comp_return_val == -2) {
            *(merged_list + ic) = *(list_1 + ia);
            ia++;
        } else {
            *(merged_list + ic) = *(list_2 + ib);
            ib++;
        }
    }

    for(; ia < len_1; ia++, ic++) {
        *(merged_list + ic) = *(list_1 + ia);
    }

    for (; ib < len_2; ib++, ic++) {
        *(merged_list + ic) = *(list_2 + ib);
    }
}