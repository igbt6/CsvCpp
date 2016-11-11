==============================================================
CsvParser
==============================================================

Description
===========

A C++ CSV (character-seperated value, also known as comma-seperated value) parser which can both decode/encode CSV files.

Installation
============
To include CsvCpp into your embedded (or otherwise) firmware/software project, copy the repo into your project folder (or other suitable place) and include the file :code:`/api/CsvParser.h` from your C++ code.


Examples
========

Following are some basic examples

Reading A CSV File Into A CsvTable Object And Then Printing Out Each Field - Do not forget to uncomment #define EXCEPTIONS_ALLOWED macro
-------------------------------------------------------------------------- 

::

	#include "api/CsvParser.h"

	
	int main()
	{
		Csv::Parser csvParser;
		

		// Attempt to read the entire file into the csvTable object.
		Csv::CsvTable csvTable;
		try
		{
			csvParser.readCsvFile("test.csv",csvTable);
		}
		catch(std::exception& e)
		{
			std::cout << e.what();
			// Something bad happened
			return -1;
		}

		// Print out the CSV table, record by record, field by field
		uint32_t x, y;

		for(x = 0; x < csvTable.getRowNumbers(); x++)
		{
			for(y = 0; y < csvTable[x].getNumOfFields(); y++)
			{
				std::cout << "csvTable[" << x << "][" << y << "] = " << csvTable[x][y] << std::endl;

			}
		}
		
		// Parsing successful!
		return 0;
	}
	
Writing A CSV File
------------------

::

	#include "api/CsvParser.h"
	
	int main()
	{
	
		//CSV Creator:
		Csv::CsvParser parser;
		Csv::CsvTable table;

		Csv::CsvRow row;
		row.addField("Test 1");
		row.addField("Test 2");
		row.addField("Test 3");

		table.addRow(row);
		row.clearAll();

		row.addField("Test 33");
		row.addField("Test 333");
		row.addField("Test 333");

		table.addRow(row);
		row.clearAll();

		parser.createCsvFile("test.csv",table);
	}
	
