
#include <string.h>
#include <stdio.h>

#include "bignum.h"

void test_add_mul()
{
/*
//test result:
	A = 123456789012345678901234567890
	B = 123456789012345678901234567890
	X = A + B
	X = 246913578024691357802469135780

	A = 123456789012345678901234567890
	B = 123456789012345678901234567890
	X = A * B
	X = 15241578753238836750495351562536198787501905199875019052100
  
	press any key to contiue ...
*/
	int ret, temp;
    mpi A, B, X;
	size_t n;
    char a[ 2 * POLARSSL_MPI_MAX_SIZE + 2 ];
    char b[ 2 * POLARSSL_MPI_MAX_SIZE + 2 ];
    char x[ 2 * POLARSSL_MPI_MAX_SIZE + 2 ];

	n = sizeof(a);
    temp = n - 2;	
		
    mpi_init( &A ); mpi_init( &B ); mpi_init( &X );
	
    MPI_CHK( mpi_read_string( &A, 10,
        "123456789012345678901234567890") );
	
    MPI_CHK( mpi_read_string( &B, 10,
        "123456789012345678901234567890") );
	
    MPI_CHK( mpi_add_mpi( &X, &A, &B ) );

    memset( a, 0, sizeof(a) );
    memset( b, 0, sizeof(b) );
    memset( x, 0, sizeof(x) );
	n = temp;
    MPI_CHK( mpi_write_string( &A, 10, a, (size_t *) &n ) );	
	n = temp;
    MPI_CHK( mpi_write_string( &B, 10, b, (size_t *) &n ) );	
	n = temp;
    MPI_CHK( mpi_write_string( &X, 10, x, (size_t *) &n ) );	
	printf("A = %s\n", a);
	printf("B = %s\n", b);
	printf("X = A + B\n");
	printf("X = %s\n", x);
	printf("\n");

	MPI_CHK( mpi_mul_mpi( &X, &A, &B ) );
	
    memset( a, 0, sizeof(a) );
    memset( b, 0, sizeof(b) );
    memset( x, 0, sizeof(x) );
	n = temp;
    MPI_CHK( mpi_write_string( &A, 10, a, (size_t *) &n ) );	
	n = temp;
    MPI_CHK( mpi_write_string( &B, 10, b, (size_t *) &n ) );	
	n = temp;
    MPI_CHK( mpi_write_string( &X, 10, x, (size_t *) &n ) );
	printf("A = %s\n", a);
	printf("B = %s\n", b);
	printf("X = A * B\n");
	printf("X = %s\n", x);
	printf("\n");
		
cleanup:
	
    mpi_free( &A ); mpi_free( &B ); mpi_free( &X );
}

void my_pause()
{
	printf("press any key to contiue ...");
	getchar();
}

int main( int argc, char *argv[] )
{

	test_add_mul();

	my_pause();

    return 0;
}
