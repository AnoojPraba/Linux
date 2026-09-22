#include <iostream>
#include <vector>
#include <list>

/*****************************************************************************
 * Name: demonstrateVectorReallocationInvalidation
 *
 * Description:
 *         Shows that a vector reallocation (triggered by exceeding
 *         capacity) invalidates all existing iterators/pointers/references,
 *         because the elements move to a new backing array.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void demonstrateVectorReallocationInvalidation()
{
    std::vector<int> values;
    values.reserve(1);
    values.push_back(1);

    auto iteratorBeforeGrowth = values.begin();
    // pushing past capacity forces a reallocation; iteratorBeforeGrowth is now
    // dangling and must not be dereferenced.
    values.push_back(2);
    (void)iteratorBeforeGrowth;

    std::cout << "vector reallocation invalidates prior iterators/pointers/references\n";
}

/*****************************************************************************
 * Name: demonstrateVectorEraseInvalidation
 *
 * Description:
 *         Shows that vector::erase invalidates the erased element's
 *         iterator and every iterator after it (elements shift down), but
 *         the returned iterator from erase() remains valid to continue from.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void demonstrateVectorEraseInvalidation()
{
    std::vector<int> values = { 1, 2, 3, 4, 5 };
    auto it = values.begin() + 1;
    // erase returns a valid iterator to the element that followed the erased one;
    // any other iterator held past this point (besides the returned one) is invalid.
    it = values.erase(it);
    std::cout << "element after erase = " << *it << "\n";
}

/*****************************************************************************
 * Name: demonstrateListStability
 *
 * Description:
 *         Contrasts vector with std::list: inserting or erasing other
 *         elements in a list never invalidates iterators to elements that
 *         were not themselves erased, because list nodes are not
 *         contiguous and are not moved.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void demonstrateListStability()
{
    std::list<int> values = { 1, 2, 3, 4, 5 };
    auto it = std::next(values.begin(), 2);
    values.push_back(6);
    values.push_front(0);
    // it still safely refers to the same element (value 3), unlike a vector iterator
    // that could have been invalidated by growth.
    std::cout << "list iterator still valid, points to " << *it << "\n";
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Runs each iterator-invalidation demonstration in turn.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    demonstrateVectorReallocationInvalidation();
    demonstrateVectorEraseInvalidation();
    demonstrateListStability();

    return 0;
}
