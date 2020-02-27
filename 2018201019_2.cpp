/* MPI Program Template */                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      /* MPI Program Template */

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

const int SIZE = 10000, MAXVAL = 999999; //maximum size of the graph

int N, Source;
int G[SIZE][SIZE]; //adjacency matrix
int D[SIZE], path[SIZE]; //distance
bool OK[SIZE]; //nodes done

void writeFile(string fileName){
    ofstream output_file;
    string output_file_name = fileName;
    output_file.open(output_file_name.c_str());
    if(output_file.is_open()){
        FOR0(i, N){
            int node = i + 1;
            output_file << node << " ";
            if(D[i] != MAXVAL)
                output_file << D[i];
            else
                output_file << "-1";
            output_file<<endl;
        }
    }
    output_file.close();
}

void readFile(string fileName) {
    ifstream input_file;
    int word;
    input_file.open(fileName);

    int M;
    if(input_file.is_open()){
        input_file >> N >> M;
        FOR0(i, N) {
            FOR0(j, N)
                G[i][j] = MAXVAL;
        }
        int x, y, cost;
        FOR0(i, M) {
            input_file >> x;
            input_file >> y;
            input_file >> cost;
            x--, y--;
            G[y][x] = G[x][y] = cost;
        }
        input_file >> Source;
        Source--;
    }
    input_file.close();
}

void Dijkstra(int s, int numprocs, int rank) {
    int x = 0;
    for(int i = rank; i < N; ){
        path[i] = s;
        D[i] = G[s][i];
        OK[i] = 0;
        i = i + numprocs;
    }
    D[s] = 0;
    path[s] = -1;
    OK[s] = 1;
    FORl(j, 1, N){

        struct {
            int dd, xx;
        } point{}, temp_point{};
        int tmp = 9999999;

        for(int i = rank; i < N; ) {
            if (!OK[i] and D[i] < tmp){
                tmp = D[i];
                x = i;
            }
            i = i + numprocs;
        }
        point.xx = x, point.dd = tmp;

        MPI_Allreduce(&point, &temp_point, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
        
        OK[temp_point.xx] = 1;
        x = temp_point.xx;
        D[x] = temp_point.dd;
        for(int i = rank; i < N; ) {
            if (!OK[i] and D[i] > D[x] + G[x][i]){
                path[i] = x;
                D[i] = D[x] + G[x][i];
            }
            i = i + numprocs;
        }
    }
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
    if (!rank) 
        readFile(argv[1]);
    
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int TOTAL = SIZE * SIZE;
    MPI_Bcast(G, TOTAL, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&Source, 1, MPI_INT, 0, MPI_COMM_WORLD);

    Dijkstra(Source, numprocs, rank);

    if (!rank) 
        writeFile(argv[2]);

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