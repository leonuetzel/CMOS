#pragma once

#include "string.hpp"
#include "matrix.hpp"





class CSV: public Matrix<String>
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		CSV();
		CSV(uint32 numberOfColumns, uint32 numberOfRows);
		CSV(const CSV& file, const Array<String>& validHeaders);
		CSV(String data, char columnSeparator = ';');
		
		template<typename dataType>
		CSV(const Matrix<dataType>& data, String (*toStringFunction)(dataType value) = nullptr, const Array<String>& header = Array<String>());
		
		~CSV();
		
		uint32 get_columnNumber(String columnName) const;
		Array<String> get_columnByHeader(String columnName) const;
		void append(const CSV& csv, bool appendWithoutHeader = false);
		
		String transformToString(char columnSeparator = ';') const;
		
		CSV& operator+=(const CSV& csv);
		CSV operator+(const CSV& csv) const;
};




template<typename dataType>
CSV::CSV(const Matrix<dataType>& data, String (*toStringFunction)(dataType value), const Array<String>& header)
	:	Matrix(data.get_numberOfColumns(), data.get_numberOfRows())
{
	uint32 startRow = 0;
	if(header.get_size() > 0)
	{
		startRow = 1;
		set_row(0, header);
		Array<String> emptyRow;
		append_row(emptyRow);
	}
	
	
	const uint32 c_numberOfRows = get_numberOfRows();
	const uint32 c_numberOfColumns = get_numberOfColumns();
	
	
	for(uint32 row = startRow; row < c_numberOfRows; row++)
	{
		for(uint32 column = 0; column < c_numberOfColumns; column++)
		{
			if(toStringFunction == nullptr)
			{
				set_cell(column, row, String(data.get_cell(column, row - startRow)));
			}
			else
			{
				set_cell(column, row, toStringFunction(data.get_cell(column, row - startRow)));
			}
		}
	}
}