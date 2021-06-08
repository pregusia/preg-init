/*
 * algorithm.h
 *
 *  Created on: 18 kwi 2016
 *      Author: pregusia
 */

#ifndef INCLUDE_STDEXT_ALGORITHM_H_
#define INCLUDE_STDEXT_ALGORITHM_H_

#include <vector>

namespace stdext {


	template<typename T>
	void insertionSort(std::vector<T>& arr) {
		if (arr.size() <= 1) return;
	    for(int32_t i = 1 ; i < (int32_t)arr.size() ; i++) {
	        T value = arr[i];
	        int32_t j = i;
	        while(j > 0 && arr[j - 1] > value) {
	        	arr[j] = arr[j - 1];
	        	j -= 1;
	        }
	        arr[j] = value;
	    }
	}

	template<typename T, typename C>
	void insertionSort(std::vector<T>& arr, const C& comparer) {
		if (arr.size() <= 1) return;
	    for(int32_t i = 1 ; i < (int32_t)arr.size() ; i++) {
	        T value = arr[i];
	        int32_t j = i;
	        while(j > 0 && !comparer(arr[j - 1],value)) {
	        	arr[j] = arr[j - 1];
	        	j -= 1;
	        }
	        arr[j] = value;
	    }
	}

}

#endif /* INCLUDE_STDEXT_ALGORITHM_H_ */
