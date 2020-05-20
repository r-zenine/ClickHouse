#include "Algorithms.h"
#include "GatherUtils.h"
#include "Selectors.h"

namespace DB::GatherUtils
{
struct ArraySortedSearchSelector : public ArrayAndValueSourcePairSelector<ArraySortedSearchSelector>
{
    template <typename FirstSource, typename SecondSource>
    static void selectSourcePair(FirstSource && first, SecondSource && second, ColumnUInt8 & result)
    {
        arraySortedSearch(first, second, result);
    }
};

void sortedSearch(IArraySource & first, IValueSource & second, ColumnUInt8 & result)
{
    ArraySortedSearchSelector::select(first, second, result);
}

}
