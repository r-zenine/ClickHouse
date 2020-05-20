#include <Functions/IFunctionImpl.h>
#include <Functions/FunctionFactory.h>
#include <Functions/GatherUtils/GatherUtils.h>
#include <DataTypes/DataTypeArray.h>
#include <DataTypes/DataTypesNumber.h>
#include <Columns/ColumnArray.h>
#include <Columns/ColumnConst.h>
#include <Interpreters/castColumn.h>
#include <Common/typeid_cast.h>


namespace DB
{

namespace ErrorCodes
{
    extern const int LOGICAL_ERROR;
    extern const int ILLEGAL_TYPE_OF_ARGUMENT;
}


class FunctionArraySearch : public IFunction
{
public:
    static FunctionPtr create(const Context &) { return std::make_shared<FunctionArraySearch>(); }
    String getName() const override { return name; }

    bool isVariadic() const override { return false; }
    size_t getNumberOfArguments() const override { return 2; }

    DataTypePtr getReturnTypeImpl(const DataTypes & arguments) const override
    {
        if (arguments[0]->onlyNull())
            throw Exception("First argument for function " + getName() + " must contain at least one non null element.", ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);

        auto array_type = typeid_cast<const DataTypeArray *>(arguments[0].get());
        if (!array_type)
            throw Exception("First argument for function " + getName() + " must be an array but it has type "
                            + arguments[0]->getName() + ".", ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);

        auto nested_type = array_type->getNestedType();

        DataTypes types = {nested_type, arguments[1]};

        return std::make_shared<DataTypeUInt8>();
    }

    void executeImpl(Block & block, const ColumnNumbers & arguments, size_t result, size_t input_rows_count) override
    {
        auto array_column = block.getByPosition(arguments[0]).column;
        auto search_column = block.getByPosition(arguments[1]).column;

        std::unique_ptr<GatherUtils::IArraySource> array_source;
        std::unique_ptr<GatherUtils::IValueSource> value_source;

        bool is_const = false;
        if (auto const_array_column = typeid_cast<const ColumnConst *>(array_column.get()))
        {
            is_const = true;
            array_column = const_array_column->getDataColumnPtr();
        }

        if (auto argument_column_array = typeid_cast<const ColumnArray *>(array_column.get()))
            array_source = GatherUtils::createArraySource(*argument_column_array, is_const, input_rows_count);
        else
            throw Exception{"First arguments for function " + getName() + " must be array.", ErrorCodes::LOGICAL_ERROR};


        bool is_search_const = false;
        if (auto const_searched_column = typeid_cast<const ColumnConst *>(search_column.get()))
        {
            is_search_const = true;
            search_column = const_searched_column->getDataColumnPtr();
        }

        value_source = GatherUtils::createValueSource(*search_column, is_search_const, input_rows_count);

        auto result_column = ColumnUInt8::create(input_rows_count);
        auto result_column_ptr = typeid_cast<ColumnUInt8 *>(result_column.get());
        GatherUtils::sortedSearch(*array_source, *value_source, *result_column_ptr);
        block.getByPosition(result).column = std::move(result_column);
    }

    bool useDefaultImplementationForConstants() const override { return true; }
    bool useDefaultImplementationForNulls() const override { return false; }

    static constexpr auto name = "arraySearch";
};

void registerFunctionArraySearch(FunctionFactory & factory)
{
    factory.registerFunction<FunctionArraySearch>();
}


}
