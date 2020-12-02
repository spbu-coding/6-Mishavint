#include <stdio.h>
#include <stdlib.h>
#include "sortings.h"

#define error_for_malloc fprintf( stderr , "Can not take memory" );

struct users_params
{
    array_size_t number_of_lines;
    char *first_file;
    char *second_file;
    int sorting_type; // 1 - bubble , 2 - insertion , 3 - merge , 4 - quick , 5 - radix
    comparator_func_t comparer_type;
};

int asc_comparor(const char* first_line, const char* second_line)
{
    return strcmp(first_line, second_line );
}

int des_comparor(const char* first_line, const char* second_line)
{
    return -strcmp(first_line, second_line);
}


int test_params(int const* number_of_params , char **argv , struct users_params *params_to_write )
{
    if( *number_of_params < 6 )
    {
        fprintf( stderr , "You have entered less, than 5 params" );
        return -1;
    }
    if( *number_of_params > 6 )
    {
        fprintf( stderr , "You have entered more, than 5 params" );
        return -1;
    }


    params_to_write->number_of_lines = (int)strtol(argv[ 1 ] , NULL , 10 ) ;
    if( (int)params_to_write->number_of_lines < 0 )
    {
        fprintf( stderr , "You have entered less than 1 line" );
        return -1;
    }


    int temp_for_params_length = (int)strlen(argv[2] );
    if((temp_for_params_length <= 4 ) || (strncmp((argv[ 2 ] + (temp_for_params_length - 4)) , ".txt" , 4) != 0 ) )
    {
        fprintf( stderr , "Please enter valid <input_name>.txt" );
        return -1;
    }

    temp_for_params_length = (int)strlen(argv[3] );
    if((temp_for_params_length <= 4 ) || (strncmp((argv[ 3 ] + (temp_for_params_length - 4)) , ".txt" , 4) != 0 ) )
    {
        fprintf( stderr , "Please enter valid <output_name>.txt" );
        return -1;
    }

    temp_for_params_length = (int)strlen(argv[5]);
    if((temp_for_params_length != 3 ) || (strncmp(argv[5] , "asc" , 3 ) != 0 &&
                                            strncmp(argv[5] , "des" , 3 ) != 0))
    {
        fprintf( stderr , "Please enter valid comparer" );
        return -1;
    }

    params_to_write->first_file = argv[2];
    params_to_write->second_file = argv[3];

    return 0;
}

int parcing_params( char **argv ,  struct users_params *params_to_write )
{
    if( strncmp( argv[5] , "asc"  ,3 ) ==0 )
    {
        params_to_write->comparer_type = asc_comparor;
    }
    else
    {
        params_to_write->comparer_type = des_comparor;
    }


    int temp_for_params_length;
    temp_for_params_length = (int)strlen( argv[4] );

    switch ( temp_for_params_length )
    {
        case 6:
            if( strncmp( argv[4] , "bubble" , 6 ) != 0 )
            {
                fprintf( stderr , "Please, enter valid sorting type" );
                return -1;
            }
            else
            {
                params_to_write->sorting_type = 1;
            }
            break;
        case 9:
            if( strncmp( argv[ 4 ] , "insertion" , 9 ) != 0 )
            {
                fprintf( stderr , "Please, enter valid sorting type" );
                return -1;
            }
            else
            {
                params_to_write->sorting_type = 2;
            }
            break;
        case 5:
            if( ( strncmp( argv[ 4 ] , "merge" , 5 ) != 0 ) && (strncmp( argv[4]  , "quick" , 5 ) != 0)
            && ( strncmp( argv[ 4 ] , "radix" , 5 ) != 0) )
            {
                fprintf( stderr , "Please, enter valid sorting type" );
                return -1;
            }
            break;
        default:
            fprintf( stderr , "Please, enter valid sorting type" );
            return -1;
    }

    if( strncmp( argv[4] , "merge" , 5 ) == 0 )
    {
        params_to_write->sorting_type = 3;
    }
    else if( strncmp( argv[4] , "quick" , 5 ) == 0 )
    {
        params_to_write->sorting_type = 4;
    }
    else if( strncmp( argv[4] , "radix" , 5 ) == 0 )
    {
        params_to_write->sorting_type = 5;
    }

    return 0;
}

void free_array_of_lines( strings_array_t array , array_size_t const* number_of_lines )
{
    for( int i = 0 ; i < (int)*number_of_lines ; i++ )
    {
        free( array[i] );
    }
    free( array );
}

int main( int argc , char **argv )
{
    struct users_params params = { 0 , NULL , NULL , 0 , 0};
    int check_for_errors_with_params = test_params( &argc , argv  ,  &params );
    if( check_for_errors_with_params != 0 )
    {
        return check_for_errors_with_params;
    }
    check_for_errors_with_params = parcing_params( argv , &params );
    if( check_for_errors_with_params != 0 )
    {
        return check_for_errors_with_params;
    }


    FILE *input_file;
    input_file = fopen( params.first_file , "rb" );
    if( input_file == NULL )
    {
        fprintf( stderr , "PLease, enter valid input file" );
        return -1;
    }

    strings_array_t array_of_lines ;

    array_of_lines = (char**)malloc( sizeof( char ) * params.number_of_lines );
    if(array_of_lines == NULL )
    {
        error_for_malloc
        return -1;
    }
    char *temp_for_first_line = (char*)malloc( sizeof(char) * MAX_INPUT_STRING_SIZE );  // Than line numbers more, 1st line is bugging. This is my fix

    for( array_size_t i = 0 ; i < params.number_of_lines  ; i++ )
    {
        array_of_lines[i] = (char*)malloc( sizeof( char ) * MAX_INPUT_STRING_SIZE );
        if(*array_of_lines == NULL )
        {
            error_for_malloc
            free_array_of_lines( array_of_lines , &i );
            return -1;
        }
        if ( fgets(array_of_lines[i] , MAX_INPUT_STRING_SIZE , input_file ) == NULL)
        {
            fprintf( stderr , "You have entered more lines, than in file" );
            return -1;
        }
        if( i == 0 && params.number_of_lines >= 5 )
        {
            temp_for_first_line = array_of_lines[0];
        }
    }

    if( params.number_of_lines >=5 )
    {
        array_of_lines[0] = temp_for_first_line;
    }

    switch ( params.sorting_type )
    {
        case 1:
            bubble( array_of_lines , params.number_of_lines , params.comparer_type );
            break;
        case 2:
            insertion( array_of_lines , params.number_of_lines , params.comparer_type );
            break;
        case 3:
            merge( array_of_lines , params.number_of_lines , params.comparer_type );
            break;
        case 4:
            quick( array_of_lines , params.number_of_lines , params.comparer_type );
            break;
        case 5:
            radix( array_of_lines , params.number_of_lines , params.comparer_type );
            break;
    }

    FILE *output_file;

    output_file = fopen( params.second_file , "wb" );
    if( output_file == NULL )
    {
     fprintf( stderr , " Can not open %s " , params.second_file );
     return -1;
    }

    for( int i = 0 ; i < (int)params.number_of_lines ; i++ )
    {
        printf("[%d] = %s\n" , i , array_of_lines[i]);
    }

    for( int i = 0 ; i < (int)params.number_of_lines ; i++ )
    {
        fputs( array_of_lines[i] , output_file );
    }
    if( params.number_of_lines == 0 )
    {
        fputs( "\n" , output_file );
    }

    fclose( output_file );
    fclose( input_file );
    free_array_of_lines( array_of_lines , &params.number_of_lines);
    return 0;
}
