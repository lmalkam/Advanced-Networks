#include<bits/stdc++.h>
using namespace std;
#define ll long long int
#define f(i,n) for(ll i=1;i<=n;i++)
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL);
#define MOD 1000000007
ll j=1,a[100];

bool isTwo (ll x)
{
  return x && (!(x&(x-1)));
}

void isSet(ll ch[], ll n, ll k)
{
  ll ans=0;
  f(i,n)
  if (i & (1 << (k - 1)))
      ans=ans^ch[i];

  ch[1 << (k - 1)]=ans;
}

ll countBits(ll number)
{
      return (ll)log2(number)+1;
}

void toInt(string l , ll n)
{
  ll j=0;

  f(i,n)
  {
    if(isTwo(i))
    {
      a[i]=0;
    }
    else
    {
      if(l[j]=='1')
      a[i]=1;

      j++;

    }
  }

}

int main()
{
  ll n; string s;
  cout<<"Enter the code: ";
  cin>>s;

  n=s.length();

  ll r=countBits(n+countBits(n));
  n=n+r;

  toInt(s,n);

  f(k,r)
    isSet(a,n,k);

  cout<<"\nHamming code : ";

  f(i,n)
  cout<<a[i];

}
