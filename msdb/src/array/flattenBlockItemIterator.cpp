#include <pch.h>
#include <array/flattenBlockItemIterator.h>

namespace msdb
{
namespace core
{
flattenBlockItemIterator::flattenBlockItemIterator(void* data,
                                           const eleType eType, 
                                           const dimension& dims,
                                           const dimension& bSp,
                                           pBitmap itemBitmap)
    : blockItemIterator(data, eType, dims, bSp, itemBitmap),
    mdItr(dims.size(), dims.data())
{
    if(itemBitmap == nullptr)
    {
        this->itemBitmap_ = std::make_shared<bitmap>(dims.area(), true);
    }
}

flattenBlockItemIterator::flattenBlockItemIterator(void* data,
                                           const eleType eType,
                                           const dimension& dims,
                                           const range& irange,
                                           const dimension& bSp,
                                           pBitmap itemBitmap)
    : blockItemIterator(data, eType, dims, irange, bSp, itemBitmap),
    mdItr(dims.size(), dims.data())
{
    if (itemBitmap == nullptr)
    {
        this->itemBitmap_ = std::make_shared<bitmap>(dims.area(), true);
    }
}

flattenBlockItemRangeIterator::flattenBlockItemRangeIterator(void* data,
                                                     const eleType eType,
                                                     const dimension& dims, 
                                                     const range& range, 
                                                     const dimension& bSp,
                                                     pBitmap itemBitmap)
    : blockItemRangeIterator(data, eType, dims, range, bSp, itemBitmap),
    mdItr(dims.size(), dims.data())
{
    if (itemBitmap == nullptr)
    {
        this->itemBitmap_ = std::make_shared<bitmap>(dims.area(), true);
    }
}
}		// core
}		// msdb