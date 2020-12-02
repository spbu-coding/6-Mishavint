#include "sortings.h"

#define ASCII 128

void change_lines( char** str1 , char** str2 )
{
    char *temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}

void bubble( strings_array_t array , array_size_t number_of_lines , comparator_func_t comparer_type )
{
    for( int i = 0 ; i < (int)number_of_lines; i++ )
    {
        for( int j = 0 ; j < (int)number_of_lines - 1 ; j++ )
        {
            if( comparer_type( array[j] , array[j+1] ) > 0 )
            {
                change_lines( &array[j] , &array[j+1] );
            }
        }
    }
}

void insertion( strings_array_t array , array_size_t number_of_lines , comparator_func_t comparer_type )
{
    for( int i = 1 ; i < (int)number_of_lines ; i++ )
    {
        char *temp = array[i];
        int j;
        for( j = i ; j > 0 && comparer_type( temp , array[j-1] ) < 0 ; j-- )
        {
            array[j] = array[j-1];
        }
        array[j] = temp;
    }
}

void main_merge(size_t left, size_t mid, size_t right, strings_array_t strings_array, comparator_func_t comparer_type){
    char* temp[right - left + 1];
    int left_b = left;
    int right_b = (int)mid + 1;
    int k = 0;
    while (left_b <= (int)mid && right_b <= (int)right)
    {
        if (comparer_type(strings_array[left_b], strings_array[right_b]) < 0)
        {
            temp[k] = strings_array[left_b];
            k++;
            left_b++;
        }
        else
        {
            temp[k] = strings_array[right_b];
            k++;
            right_b++;
        }
    }

    while(left_b <= (int)mid)
    {
        temp[k] = strings_array[left_b];
        k++;
        left_b++;
    }
    while(right_b <= (int)right)
    {
        temp[k] = strings_array[right_b];
        k++;
        right_b++;
    }
    for (left_b = left; left_b <= (int)right; left_b++)
    {
        strings_array[left_b] = temp[left_b - left];
    }

}

void merge_with_recursive(strings_array_t strings_array, comparator_func_t comparer_type , size_t left, size_t right)
{
    if (left < right)
    {
        size_t mid = left + (right - left) / 2;
        merge_with_recursive(strings_array, comparer_type , left, mid );
        merge_with_recursive(strings_array, comparer_type , mid + 1, right );
        main_merge(left, mid, right, strings_array, comparer_type);
    }
}

void merge( strings_array_t array , array_size_t number_of_lines , comparator_func_t comparer_type )
{
    if( number_of_lines != 0 )
    {
        merge_with_recursive(array, comparer_type , 0, number_of_lines - 1 );
    }
    else
    {
        return;
    }
}

void quick_realisation( strings_array_t array , array_size_t number_of_lines , comparator_func_t comparer_type , int first , int last )
{
    if( first < last )
    {
        int left = first;
        int right = last;
        char *middle = array[ (left + right) / 2 ];

        do
        {
            while ( comparer_type( array[left] , middle ) < 0 ) left++;
            while ( comparer_type( array[right] , middle ) > 0 ) right--;
            if( left <= right )
            {
                change_lines( &array[left] , &array[right] );
                left ++; right--;
            }
        } while( left <= right );
        quick_realisation( array , number_of_lines , comparer_type , first , right );
        quick_realisation( array , number_of_lines , comparer_type , left , last );

    }

}

void quick( strings_array_t array , array_size_t number_of_lines , comparator_func_t comparer_type )
{
    quick_realisation( array , number_of_lines , comparer_type , 0 , (int)number_of_lines - 1 );
}

void radix_realisation( char **array , array_size_t number_of_lines , int max_radix , comparator_func_t comparer_type)
{
    int cmp = ( comparer_type( "A" , "B" ) < 0 ) ? 1 : 2 ;

    for (int i = max_radix - 1; i >= 0; i--) {
        char *temp_array[number_of_lines];
        int count[ASCII] = { 0 };

        for (int j = 0; j < (int) number_of_lines; j++) {
            count[(int) array[j][i]]++;
        }
        for (int j = 1; j < ASCII; j++) {
            count[j] += count[j - 1];
        }
        for (int j = (int)number_of_lines - 1; j >= 0; j--) {
            temp_array[count[(int) array[j][i]] - 1] = array[j];
            count[(int) array[j][i]]--;
        }
        for (int j = (int)number_of_lines - 1; j >= 0; j--) {
            if (i > 0 || cmp == 1) {
                array[j] = temp_array[j];
            }
            else {
                array[number_of_lines - 1 - j] = temp_array[j];
            }
        }
    }
}

void radix( strings_array_t array , array_size_t number_of_lines , comparator_func_t comparer_type )
{

    int max_radix = 0;
    for( int i = 0 ; i < (int)number_of_lines ; i++ )
    {
        if( max_radix < (int)strlen( array[i] ) )
        max_radix = (int)strlen(array[i]);
    }

    radix_realisation( array , number_of_lines , max_radix , comparer_type );
}
