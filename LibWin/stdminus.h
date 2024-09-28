#pragma once

#include <functional>
#include <stdlib.h>
#include <string.h>

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;
typedef unsigned short ushort;

namespace stdminus {

	template<typename T , typename U>
	struct mpair {
		T x;
		U y;
		void operator =( mpair<T , U> a ) {
			x = a.x;
			y = a.y;
		}
		bool operator ==( mpair<T , U> a ) {
			return ( x == a.x );
		}
		bool operator !=( mpair<T , U> a ) {
			return ( x != a.x );
		}
		bool operator <( mpair<T , U> a ) {
			return ( x < a.x );
		}
		bool operator <=( mpair<T , U> a ) {
			return ( x <= a.x );
		}
		bool operator >( mpair<T , U> a ) {
			return ( x > a.x );
		}
		bool operator >=( mpair<T , U> a ) {
			return ( x >= a.x );
		}
	};

	template <typename T>
	void swap ( T& a , T& b ) {
		T buf = a;
		a = b;
		b = buf;
	}
#pragma region _base_arr	
	template <typename T>
	struct _base_arr {
		T* m;
		virtual T& operator []( int i ) {
			if ( i < 0 )
				return m[len + i];
			return m[i];
		}
		virtual const T& operator []( int i ) const {
			if ( i < 0 )
				return m[len + i];
			return m[i];
		}
		void clear () {
			m = ( T* ) malloc ( 0 );
			len = 0;
		}
		void quickSort () {
			quickSort ( 0 , len - 1 );
		}
		int size () {
			return len;
		}
		void Delete ( T* h ) {
			h->~T ();
		}
		~_base_arr () {
			for ( T* h = m; m + len > h; h++ ) {
				h->~T ();
			}
			free ( m );
		}
	protected:
		int len;
	private:
		void quickSort ( int l , int r ) {
			if ( l < r ) {
				T x = m[( l + r ) / 2];
				int i = l , j = r;
				while ( i <= j ) {
					while ( m[i] < x ) i++;
					while ( m[j] > x ) j--;
					if ( i <= j ) swap<T> ( m[i++] , m[j--] );
				}
				quickSort ( l , j );
				quickSort ( i , r );
			}
		}
	};
	template <typename T>
	struct _base_arr<T*> {
		T** m;
		virtual T*& operator []( int i ) {
			if ( i < 0 )
				return m[len + i];
			return m[i];
		}
		virtual const T*& operator []( int i ) const {
			if ( i < 0 )
				return m[len + i];
			return m[i];
		}
		void clear () {
			m = ( T** ) malloc ( 0 );
			len = 0;
		}
		void quickSort () {
			quickSort ( 0 , len - 1 );
		}
		int size () {
			return len;
		}
		void Delete ( T** h ) {
			delete* h;
		}
		~_base_arr () {
			for ( T** h = m; m + len > h; h++ ) {
				delete* h;
			}
			free ( m );
		}
	protected:
		int len;
	private:
		void quickSort ( int l , int r ) {
			if ( l < r ) {
				T* x = m[( l + r ) / 2];
				int i = l , j = r;
				while ( i <= j ) {
					while ( m[i] < x ) i++;
					while ( m[j] > x ) j--;
					if ( i <= j ) swap<T*> ( m[i++] , m[j--] );
				}
				quickSort ( l , j );
				quickSort ( i , r );
			}
		}
	};
#pragma endregion
#pragma region arr
	template <typename T>
	struct arr : public _base_arr<T> {
		arr () {
			arr::len = 0;
			arr::m = ( T* ) malloc ( 0 );
		}
		arr ( const arr<T>& orig ) {
			arr::len = orig.len;
			arr::m = ( T* ) malloc ( sizeof ( T ) * arr::len );
			if ( arr::len )
				memmove ( arr::m , orig.m , sizeof ( T ) * arr::len );
		}
		void add ( T a ) {
			arr::m = ( T* ) realloc ( arr::m , arr::len * sizeof ( T ) + sizeof ( T ) );
			arr::m[arr::len++] = a;
		}
		void add ( T a , int i ) {
			arr::m = ( T* ) realloc ( arr::m , ++arr::len * sizeof ( T ) );
			if ( -i + arr::len )
				memmove ( arr::m + i + 1 , arr::m + i , ( -i + arr::len ) * sizeof ( T ) );
			arr::m[i] = a;
		}
		void remBegin () {
			if ( arr::len > 1 ) {
				Delete ( arr::m );
				memmove ( arr::m , arr::m + 1 , ( arr::len - 1 ) * sizeof ( T ) );
			}
			if ( arr::len > 0 )
				arr::m = ( T* ) realloc ( arr::m , --arr::len * sizeof ( T ) );
		}
		void rem () {
			if ( arr::len > 0 ) {
				Delete ( arr::m + arr::len - 1 );
				arr::m = ( T* ) realloc ( arr::m , --arr::len * sizeof ( T ) );
			}
		}
		void rem ( T* h ) {
			if ( ( arr::m + arr::len - h - 1 ) > 0 )
				memmove ( h , h + 1 , ( arr::m + arr::len - h - 1 ) * sizeof ( T ) );
			if ( arr::len > 0 ) {
				Delete ( h );
				arr::m = ( T* ) realloc ( arr::m , --arr::len * sizeof ( T ) );
			}
		}
		void rem ( T h ) {
			for ( int i = 0; i < arr::len; i++ )
				if ( h == arr::m[i] ) {
					rem ( arr::m + i );
					break;
				}
		}
		/// Post-function for quickSort
		int binFound ( T a ) {
			int l = 0 , r = arr::len - 1 , mid;
			while ( l <= r ) {
				mid = ( l + r ) / 2;
				if ( *( arr::m + mid ) == a )
					return mid;
				if ( *( arr::m + mid ) < a )
					l = mid + 1;
				else
					r = mid - 1;
			}
			return r + 1;
		}
	};
#pragma endregion
#pragma region starr
	template <typename T>
	struct starr : public _base_arr<T> {
		starr ( uint l ) {
			starr::len = l;
			starr::m = ( T* ) malloc ( l * sizeof ( T ) );
		}
		starr ( starr<T>& orig ) {
			starr::len = orig.len;
			starr::m = ( T* ) malloc ( sizeof ( T ) * starr::len );
			memmove ( starr::m , orig.m , sizeof ( T ) * starr::len );
		}
		/// Post-function for quickSort
		int binFound ( T a ) {
			int l = 0 , r = starr::len - 1 , mid;
			while ( l <= r ) {
				mid = ( l + r ) / 2;
				if ( *( starr::m + mid ) == a )
					return mid;
				if ( *( starr::m + mid ) < a )
					l = mid + 1;
				else
					r = mid - 1;
			}
			return r + 1;
		}
	};
#pragma endregion

#pragma region Event
	template<class T , class... Arg>
	struct Event : public arr<std::function<T ( Arg... )>> {
		starr<T> use ( Arg... args ) {
			starr<T> res = starr<T> ( this->len );
			for ( auto i = 0; i < this->len; i++ )
				res[i] = ( *( this->m + i ) )( args );
			return res;
		}
	};

	template<class... Arg>
	struct Event<void , Arg...> : public arr<std::function<void ( Arg... )>> {
		void use ( Arg... a ) {
			for ( auto i = 0; i < this->len; i++ )
				this->m[i] ( a );

		}
	};
#pragma endregion


#pragma region set
	template <typename T>
	struct set : public _base_arr<T> {
		set () {
			set::len = 0;
			set::m = ( T* ) malloc ( 0 );
		}
		T has ( T a ) {
			T* it = binFound ( a );
			if ( !set::len || *( it - 1 ) != a || ( it - 1 ) < set::m )
				return 0;
			return *it;
		}
		void add ( T a ) {
			T* it = binFound ( a );
			//!map::len || (it - 1)->x != i || (it - 1) < map::m
			if ( !set::len || *( it - 1 ) != a || ( it - 1 ) < set::m )
				multyAdd ( a , it );
			/*if ((it - 1) < set::m || *(it - 1) == a)
				return;
			multyAdd(a, it);*/
		}
		T* multyAdd ( T a , T* it ) {
			T* old = set::m;
			set::m = ( T* ) realloc ( set::m , ++set::len * sizeof ( T ) );
			it = ( T* ) ( ( ll ) it - ( ll ) old + ( ll ) set::m );
			ll l;
			if ( ( l = ( ( ll ) set::m - ( ll ) it ) / ( ll ) sizeof ( T ) + set::len - 1 ) )
				memmove ( it + 1 , it , ( ( ( ll ) set::m - ( ll ) it ) / ( ll ) sizeof ( T ) + set::len - 1 ) * sizeof ( T ) );
			*it = a;
			return it;
		}
		void rem ( T a ) {
			if ( !set::len )
				return;
			T* it = binFound ( a );
#if !std::is_fundamental_v<T>
			it->~T ();
#endif
			if ( ( it - 1 ) < set::m || *( it - 1 ) != a )
				return;
			if ( set::m - it + set::len > 0 )
				memmove ( it - 1 , it , ( set::m - it + set::len ) * sizeof ( T ) );
			set::m = ( T* ) realloc ( set::m , --set::len * sizeof ( T ) );
		}
		T* binFound ( T a ) {
			int l = 0 , r = set::len - 1 , mid = 0;
			while ( l <= r ) {
				mid = ( l + r ) / 2;
				if ( *( set::m + mid ) <= a && ( mid + 1 == set::len || *( set::m + mid + 1 ) > a ) )
					return set::m + mid + 1;
				if ( *( set::m + mid ) <= a )
					l = mid + 1;
				else
					r = mid - 1;
			}
			return set::m + mid;
		}
		/*void print() {
			printf("\n");
			for (int* h = (int*)set::m; h != set::m + set::len; h++)
				printf("%d ", *h);
			printf("\n");
		}*/
	};
#pragma endregion
#pragma region map
	template<typename T2 , typename U>
	struct map : public set<mpair<T2 , U>> {
		std::function<U ()> init;
		map ( std::function<U ()> func ) {
			map::len = 0;
			map::m = ( mpair<T2 , U>* )malloc ( 0 );
			init = func;
		}
		void remMap ( T2 a ) {
			if ( !map::len )
				return;
			mpair<T2 , U> buf;
			buf.x = a;
			buf.y = init ();
			map::rem ( buf );
		}
		U& operator []( T2 i ) {
			mpair<T2 , U> buf;
			buf.x = i;
			buf.y = init ();
			mpair<T2 , U>* it = map::binFound ( buf );
			if ( !map::len || ( it - 1 )->x != i || ( it - 1 ) < map::m ) {
				it = map::multyAdd ( buf , it );
			}
			else
				return ( it - 1 )->y;

			return it->y;
		}
		void prnt () {
			for ( int i = 0; i < map::len; i++ ) {
				printf ( "%d -> %d\n" , ( map::m + i )->x , ( map::m + i )->y );
			}
			printf ( "\n" );
		}
	};

	template<typename U>
	struct map<int , U> : public set<mpair<int , U>> {
		std::function<U ()> init;
		map ( std::function<U ()> func ) {
			map::len = 0;
			map::m = ( mpair<int , U>* )malloc ( 0 );
			init = func;
		}
		void remMap ( int a ) {
			if ( !map::len )
				return;
			mpair<int , U> buf;
			buf.x = a;
			buf.y = init ();
			map::rem ( buf );
		}
		mpair<int , U>& operator []( int i ) override {
			mpair<int , U> buf;
			buf.x = i;
			buf.y = init ();
			mpair<int , U>* it = map::binFound ( buf );
			if ( !map::len || ( it - 1 )->x != i || ( it - 1 ) < map::m ) {
				it = map::multyAdd ( buf , it );
			}
			else
				return *( it - 1 );

			return *it;
		}
		void prnt () {
			for ( int i = 0; i < map::len; i++ ) {
				printf ( "%d -> %d\n" , ( map::m + i )->x , ( map::m + i )->y );
			}
			printf ( "\n" );
		}
	};
#pragma endregion

#pragma region WEvents
	template<class T , class... Arg>
	struct WEvents : public map<int , arr<std::function<T ( Arg... )>>> {
		WEvents () : map<int , arr<std::function<T ( Arg... )>>> ( []()->arr<std::function<T ( Arg... )>> {return arr<std::function<T ( Arg... )>> (); } ) {

		}
		arr<std::function<T ( Arg... )>>* has ( T a ) {
			mpair<int , arr<std::function<T ( Arg... )>>> buf;
			buf.x = a;
			mpair<int , arr<std::function<T ( Arg... )>>>* it = WEvents::binFound ( buf );
			if ( !WEvents::len || ( it - 1 )->x != a || ( it - 1 ) < WEvents::m )
				return 0;
			return &it->y;
		}
	};
#pragma endregion

#pragma region geometry
	struct point {
		double x;
		double y;
		int operator<( point a ) {
			return x < a.x;
		}
		int operator>( point a ) {
			return x > a.x;
		}
		point operator+( point a ) {
			return point{ x + a.x, y + a.y };
		}
		point operator-( point a ) {
			return point{ x - a.x, y - a.y };
		}
		point operator/( double a ) {
			return point{ x / a, y / a };
		}
		void operator/=( double a ) {
			x /= a;
			y /= a;
		}
		void operator+=( point a ) {
			x += a.x;
			y += a.y;
		}
	};
	struct vec {
		point p1 , p2;
		vec ( point ap1 , point ap2 ) {
			p1 = ap1;
			p2 = ap2;
		}
		double mod () {
			return sqrt ( x () * x () + y () * y () );
		}
		vec operator+( vec a ) {
			return vec{ p1, p2 + point{a.x (), a.y ()} };
		}
		void operator+=( vec a ) {
			p2 += point{ a.x (), a.y () };
		}
		double operator*( vec a ) {
			return x () * a.x () + y () * a.y ();
		}
		double operator/( vec a ) {
			return x () * a.y () - y () * a.x ();
		}
		vec operator*( double a ) {
			return vec ( point{ 0., 0. } , point{ x () * a, y () * a } );
		}
		vec operator/( double a ) {
			return vec ( point{ 0., 0. } , point{ x () / a, y () / a } );
		}
		double r ( point a ) {
			vec n ( point{ 0., 0. } , point{ p2.y - p1.y, p1.x - p2.x } );
			vec m ( point{ a.x, a.y } , point{ p1.x, p1.y } );

			return abs ( m / *this ) / mod ();
		}
		double x () {
			return p2.x - p1.x;
		}
		double y () {
			return p2.y - p1.y;
		}
	};
#pragma endregion

#pragma region str
	struct str
	{
		char* m;
		operator char* ( ) {
			return m;
		}
	};
#pragma endregion
}