#pragma once

#include "pch.h"

#include <stdlib.h>
#include <string.h>
namespace stdminus {
	template<class T>
	void swap(T& a, T& b) {
		T t = b;
		b = a;
		a = t;
	}
	template<class T, class U>
	struct mpair {
		T x;
		U y;
	};
#pragma region map
	template<class T, class U>
	struct map {
		mpair<T, U>* m;
		int len;
		map() {
			m = (mpair<T, U>*)malloc(0);
			len = 0;
		}
		mpair<T, U>* BinariFind(T target)
		{
			if (!len)
				return 0;
			if (target < m[0].x)
				return 0;
			if (target > m[len - 1].x)
				return 0;
			int start = 0, end = len, mid = 0;
			while (start <= end) {
				mid = (start + end) / 2;
				if (m[mid].x == target)
					return m + mid;
				if (target < m[mid].x)
					end = mid - 1;
				else
					start = mid + 1;
			}
			return 0;
		}
		int add(T b) {
			int it = 0;
			if(len){
				it = BinariItFind(b);
				if ((m + it)-> x == b)
					return it;
			}
			m = (mpair<T, U>*)realloc(m, len * sizeof(T) + sizeof(T));
			if (len > 0)
				memmove(m + it + 1, m + it, (len - it) * sizeof(T));
			(*(m + it)).x = b;
			len++;
			return it;
		}
		void rem(T b) {
			mpair<T, U>* it = BinariFind(b);
			if (!it)
				return;
			rem(it);
		}
		void rem(T* it) {
			memmove(it, it + 1, ((m + len) - it) * sizeof(mpair<T, U>));

			m = (mpair<T, U>*)realloc(m, len * sizeof(mpair<T, U>) - sizeof(mpair<T, U>));

			len--;
		}
		U* has(T i) {
			mpair<T, U>* it = BinariFind(i);
			if (it)
				return &it->y;
			return 0;
		}
		U& operator[] (T i) {
			mpair<T, U>* it = BinariFind(i);
			if (!it)
				it = m + add(i);
			return it->y;
		}
	private:
		int BinariItFind(T target)
		{
			if (!len)
				return 0;
			if (target <= m[0].x)
				return 0;
			if (target > m[len - 1].x)
				return (len - 1 >= 0) ? (len) : (0);
			if (target == m[len - 1].x)
				return len - 1;
			int start = 0, end = len, mid = 0;
			while (start <= end) {
				mid = (start + end) / 2;
				if (m[mid].x == target)
					return mid;
				if (target < m[mid].x) {
					if (mid > 0 && target > m[mid - 1].x)
						return mid;
					end = mid - 1;
				}
				else {
					if (mid < len - 1 && target < m[mid + 1].x)
						return mid + 1;
					start = mid + 1;
				}
			}
			return mid;
		}
	};
#pragma endregion
#pragma region set
	template<class T>
	struct set {
		T* m;
		int len;
		set() {
			m = (T*)malloc(0);
			len = 0;
		}
		int BinariItFind(T target)
		{
			if (!len || target <= m[0])
				return 0;
			if (target > m[len - 1])
				return (len - 1 >= 0) ? (len) : (0);
			if (target == m[len - 1])
				return len - 1;
			int start = 0, end = len, mid = 0;
			while (start <= end) {
				mid = (start + end) / 2;
				if (m[mid] == target)
					return mid;
				if (target < m[mid]) {
					if (mid > 0 && target > m[mid - 1])
						return mid;
					end = mid - 1;
				}
				else {
					if (mid < len - 1 && target < m[mid + 1])
						return mid + 1;
					start = mid + 1;
				}
			}
			return mid;
		}
		T* BinariFind(T target)
		{
			if (!len || target < m[0])
				return 0;
			if (target > m[len - 1])
				return 0;
			int start = 0, end = len, mid = 0;
			while (start <= end) {
				mid = (start + end) / 2;
				if (m[mid] == target)
					return m + mid;
				if (target < m[mid])
					end = mid - 1;
				else
					start = mid + 1;
			}
			return 0;
		}
		void add(T b) {
			int it = BinariItFind(b);
			if (*(m + it) == b)
				return;

			m = (T*)realloc(m, len * sizeof(T) + sizeof(T));

			memmove(m + it + 1, m + it, (len - it) * sizeof(T));

			*(m + it) = b;

			len++;
		}
		void addMulti(T b) {
			int it = BinariItFind(b);

			m = (T*)realloc(m, len * sizeof(T) + sizeof(T));

			memmove(m + it + 1, m + it, (len - it) * sizeof(T));

			*(m + it) = b;

			len++;
		}
		void remSet(T b) {
			T* it = BinariFind(b);
			if (!it)
				return;
			remSet(it);
		}
		void remSet(T* it) {
			memmove(it, it + 1, ((m + len) - it) * sizeof(T));

			m = (T*)realloc(m, len * sizeof(T) - sizeof(T));

			len--;
		}
		T& operator[] (int i) {
			return m[i];
		}
	};
#pragma endregion
#pragma region arr
	template<class T>
	struct arr {
		T* m;
		int len;
		arr() {
			m = (T*)malloc(0);
			len = 0;
		}
		arr(arr<T>* a) {
			m = (T*)malloc(a->len * sizeof(T));
			memmove(m, a->m, a->len * sizeof(T));
			len = a->len;
		}
		void add(T b) {
			m = (T*)realloc(m, len * sizeof(T) + sizeof(T));

			*(m + len) = b;

			len++;
		}
		void rem() {
			T* h = (T*)malloc(len * sizeof(T) - sizeof(T));
			memmove(h, m, (len - 1) * sizeof(T));
			free(m);
			m = h;
		}
		void rem(T h) {
			int y = 0;
			for(int i = 0; i < len; i++){
				if (m[i] == h)
				{
					y = i;
					goto jm;
				}
			}
			return;
			jm: memmove(m + y, m + y + 1, (len - y - 1) * sizeof(T));
			m = (T*)realloc(m, (--len) * sizeof(T));
		}
		T& operator[] (int i) {
			return m[i];
		}
		void QuickSort() {
			QuickSort(0, len - 1);
		}
		void QuickSort(int l, int r) {
			if (l < r) {
				T x = m[(l + r) / 2];
				int i = l; int j = r;
				while (i <= j) {
					while (m[i] < x) i++;
					while (m[j] > x) j--;
					if (i <= j) swap<T>(m[i++], m[j--]);
				}
				QuickSort(l, j);
				QuickSort(i, r);
			}
		}
	};
#pragma endregion
#pragma region starr
	template<class T>
	struct starr {
	public:
		T* m;
		int len;
		starr(starr<T>* a) {
			m = (T*)malloc(a->len * sizeof(T));
			memmove(m, a->m, a->len * sizeof(T));
			len = a->len;
		}
		starr(int i)
		{
			m = (T*)malloc(i * sizeof(T));
			len = i;
		}
		T& operator[] (int i) {
			return m[i];
		}
		void QuickSort() {
			QuickSort(0, len - 1);
		}
		void QuickSort(int l, int r) {
			if (l < r) {
				T x = m[(l + r) / 2];
				int i = l; int j = r;
				while (i <= j) {
					while (m[i] < x) i++;
					while (m[j] > x) j--;
					if (i <= j) swap<T>(m[i++], m[j--]);
				}
				QuickSort(l, j);
				QuickSort(i, r);
			}
		}
	};
#pragma endregion
}