#include<iostream>
#include<cstdlib>
#define MAX 100

using namespace std;

int main()
{
    int n,a[MAX];
    cout<<"Enter a number N:"<<endl;
    cin>>n;
    if (n >100)
    {
        cout<<"Supported is only "<< MAX;
        return 1;
    }
    for (int i=0; i < n; i++)
        a[i]=rand();
    for (int i=0; i < n; i++)
        cout << a[i] << " ";
    cout<<endl;

    for (int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
            if(a[j]<a[i])
            {
                int temp=a[j];
                a[j]=a[i];
                a[i]=temp;
            }
        for (int i=0; i < n; i++)
            cout << a[i] << " ";
        cout<<endl;
    }


    for (int i=0; i < n; i++)
        cout << a[i] << " ";
    cout<<endl;
    return 0;
}
