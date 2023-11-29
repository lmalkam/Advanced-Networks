#include<bits/stdc++.h>
using namespace std;
#define ll long long int
#define f(i,n) for(ll i=1;i<=n;i++)
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL);
#define MOD 1000000007
ll a[100],j=1;

string HexToBin(string hexdec)
{
    ll i=0;
    string l="";
    while (hexdec[i]) {

        switch (hexdec[i]) {
        case '0':
            l=l+"0000";
            break;
        case '1':
            l=l+"0001";
            break;
        case '2':
            l=l+"0010";
            break;
        case '3':
            l=l+ "0011";
            break;
        case '4':
            l=l+ "0100";
            break;
        case '5':
            l=l+"0101";
            break;
        case '6':
            l=l+"0110";
            break;
        case '7':
            l=l+"0111";
            break;
        case '8':
            l=l+"1000";
            break;
        case '9':
            l=l+"1001";
            break;
        case 'A':
        case 'a':
            l=l+"1010";
            break;
        case 'B':
        case 'b':
            l=l+"1011";
            break;
        case 'C':
        case 'c':
            l=l+"1100";
            break;
        case 'D':
        case 'd':
            l=l+"1101";
            break;
        case 'E':
        case 'e':
            l=l+"1110";
            break;
        case 'F':
        case 'f':
            l=l+"1111";
            break;
        default:
            cout << "\nInvalid hexadecimal digit "
                 << hexdec[i];
        }
        i++;
    }

    return l;

}

void toInt(string l)
{
  for(ll k=0;k<l.length();k++)
  {
    if(l[k]=='1')
    a[j]=1;

    j++;
  }
}


ll countBits(ll number)
{
      return (ll)log2(number)+1;
}
int main()
{
  ll n; string s;
  cout<<"Enter the code: ";
  cin>>s;

  string x=HexToBin(s);                                 // converting the code

  toInt(x);                                             // to Integer array

  n=j-1;

  ll r=countBits(n);

  ll ans,p=0;

  f(k,r)
  {
    ans=0;

    f(i,n)
    if (i & (1 << (k - 1)))
        ans=ans^a[i];

    if(ans)
    p=p+(1<<(k-1));

}

  cout<<"\nError at position "<<p;
}
