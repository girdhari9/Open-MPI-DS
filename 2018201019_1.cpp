/* MPI Program Template */

#include <stdio.h>
#include "mpi.h"

#define FOR0(i,n) for(ll i=0;i<n;i++)
#define FOR1(i,n) for(ll i=1;i<=n;i++)
#define FORl(i,l,n) for(ll i=l;i<n;i++)
using namespace std;
#define pi pair<ll,ll>
#define pb push_back
#define ll long long
#define ld long double
#define ff first
#define ss second
#define mp make_pair
#define mst0(x) memset(x,0,sizeof(x));
#define vi vector<ll>
#define sync ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#define endl '\n'
#define sp <<" "<<
#define intmax 1e14
ll power(ll num,ll g,ll mod){
  if(g==0)return 1;
  if(g%2==1)return (num*power((num*num)%mod,g/2,mod))%mod;
  return power((num*num)%mod,g/2,mod);
}

using namespace std;

static ll totalSize = 0, MAX_SIZE = 10000000;

ll readFile(ll *input, string fileName){
    ifstream input_file;
    ll word;
    input_file.open(fileName);
    ll index = 0;
    if(input_file.is_open()){
        while (input_file >> word){
            input[index++] = word;
        }
    }
    totalSize = index;
    input_file.close();
    return totalSize;
}

void writeFile(ll *input, string fileName){
    ofstream output_file;
    string output_file_name = fileName;
    output_file.open(output_file_name.c_str());
    if(output_file.is_open()){
        ll index = 0;
        while (index < totalSize)
            output_file<<input[index++]<<" ";
    }
    output_file.close();
}

void _swap(ll *input, ll i, ll j){
    ll temp = input[i];
    input[i] = input[j];
    input[j] = temp;
    return ;
}

ll partition(ll *input, ll low, ll high) {
    ll pivot = input[high], i = low - 1;
    FORl(j, low, high){
        if (input[j] <= pivot) {
            ++i;
            _swap(input, i, j);
        }
    }
    _swap(input, i+1, high);
    return i + 1;
}

void QS(ll *A, ll low, ll high) {
    if (low < high) {
        ll pivot = partition(A, low, high);
        QS(A, low, pivot-1);
        QS(A, pivot + 1, high);
    }
}

ll sort_recursive(ll* arr, ll arrSize, ll currProcRank, ll maxRank, ll rankIndex) {

    ll shareProc = currProcRank + pow(2, rankIndex);
    rankIndex++;
    
    MPI_Status status;
  
    if (shareProc > maxRank || arrSize < 3) {
        ll low = 0, high = arrSize - 1;
        QS(arr, low, high);
        return 0;
    }

    ll j = 0, pivotIndex;
    do {
        pivotIndex = partition(arr, j++, arrSize - 1);
    } while (pivotIndex == j - 1);

    MPI_Send(arr, pivotIndex , MPI_LONG_LONG_INT, shareProc, pivotIndex, MPI_COMM_WORLD);

    ll arsize = (arrSize - pivotIndex - 1);
    sort_recursive((arr + pivotIndex + 1), arsize, currProcRank, maxRank, rankIndex);

    MPI_Recv(arr, pivotIndex, MPI_LONG_LONG_INT, shareProc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);   
}

int main( int argc, char **argv ) {
    int rank, numprocs;

    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */

    ll rankPower = 0;
    while (pow(2, rankPower) <= rank)
        rankPower++;
    
    MPI_Status status;

    if (rank) {

        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        int subarray_size;
        MPI_Get_count(&status, MPI_LONG_LONG_INT, &subarray_size);

        ll source_process = status.MPI_SOURCE;
        
        ll *subarray;
        subarray = (ll*)malloc(subarray_size * sizeof(ll));

        MPI_Recv(subarray, subarray_size, MPI_LONG_LONG_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       
        sort_recursive(subarray, subarray_size, rank, numprocs - 1, rankPower);

        MPI_Send(subarray, subarray_size, MPI_LONG_LONG_INT, source_process, 0, MPI_COMM_WORLD);
    }
    else {
        ll *numbers = (ll *)malloc(MAX_SIZE * sizeof(ll));

        string fileName = argv[1];
        
        ll arraySize = readFile(numbers, fileName);

        sort_recursive(numbers, arraySize, rank, numprocs - 1, rankPower);

        fileName = argv[2];
        writeFile( numbers, fileName);
    }

    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf( "Total time (s): %f\n", maxTime );
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}
