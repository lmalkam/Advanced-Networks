#include<bits/stdc++.h>
using namespace std;
#define ll long long int
#define f(i,n) for(ll i=1;i<=n;i++)
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL);
#define MOD 1000000007
ll j=1,a[100];

ll countBits(ll number)
{
      return (ll)log2(number)+1;
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

int main()
{
  ll n; string s;
  cout<<"Enter the code: ";
  cin>>s;
  toInt(s);

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

  cout<<"\nError at position "<<p<<"\n";
}
