#include "../Inc/csv.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

CSV::CSV()
{
	
}


CSV::CSV(uint32 numberOfColumns, uint32 numberOfRows)
	:	Matrix(numberOfColumns, numberOfRows)
{
	
}


CSV::CSV(const CSV& file, const Array<String>& validHeaders)
{
	const uint32 numberOfColumns = file.get_numberOfColumns();
	const uint32 numberOfValidHeaders = validHeaders.get_size();
	
	const Array<String> header(file.get_row(0));
	for(uint32 i = 0; i < numberOfColumns; i++)
	{
		for(uint32 j = 0; j < numberOfValidHeaders; j++)
		{
			if(header[i] == validHeaders[j])
			{
				Array column(file.get_column(i));
				append_column(column);
				break;
			}
		}
	}
}


CSV::CSV(String data, char columnSeparator)
{
	//	Erase all '\r' Characters from Data
	data.eraseAllOccurences('\r');
	
	
	//	Explode Data String into Rows
	Array<String> rows(data.explodeIfNotInStringQuotes('\n'));
	const uint32 numberOfRows = rows.get_size();
	
	
	//	Append the Rows
	for(uint32 i = 0; i < numberOfRows; i++)
	{
		Array<String> row(rows[i].explodeIfNotInStringQuotes(columnSeparator));
		append_row(row);
	}
}


CSV::~CSV()
{
	
}







uint32 CSV::get_columnNumber(String columnName) const
{
	const uint32 numberOfColumns = get_numberOfColumns();
	for(uint32 i = 0; i < numberOfColumns; i++)
	{
		if(get_cell(i, 0) == columnName)
		{
			return(i);
		}
	}
	return(numberOfColumns);
}


Array<String> CSV::get_columnByHeader(String columnName) const
{
	Array<String> column;
	const uint32 columnNumber = get_columnNumber(columnName);
	if(columnNumber < get_numberOfColumns())
	{
		column = get_column(columnNumber);
	}
	return(column);
}


void CSV::append(const CSV& csv, bool appendWithoutHeader)
{
	if(csv.get_numberOfRows() > 0)
	{
		CSV csvCopy(csv);
		if(appendWithoutHeader == true)
		{
			csvCopy.delete_row(0);
		}
		operator+=(csvCopy);
	}
}







String CSV::transformToString(const char columnSeparator) const
{
	String data("");
	
	const uint32 numberOfRows = get_numberOfRows();
	if(numberOfRows == 0)
	{
		return(data);
	}
	const uint32 numberOfColumns = get_numberOfColumns();
	
	
	
	for(uint32 i = 0; i < numberOfRows; i++)
	{
		const Array<String>& row = get_row(i);
		for(uint32 j = 0; j < numberOfColumns; j++)
		{
			data += row[j];
			if(numberOfColumns - j > 1)
			{
				data += columnSeparator;
			}
		}
		if(numberOfRows - i > 1)
		{
			data += '\n';
		}
	}
	return(data);
}







CSV& CSV::operator+=(const CSV& csv)
{
	Matrix::operator+=(csv);
	return(*this);
}


CSV CSV::operator+(const CSV& csv) const
{
	CSV csvReturn;
	
	csvReturn += *this;
	csvReturn += csv;
	
	return(csvReturn);
}