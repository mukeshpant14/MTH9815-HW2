#include <iostream>
#include "products.hpp"
#include "productservice.hpp"

void testFutureProductService()
{
	// Create the 10Y treasury note
	date maturityDate(2025, Nov, 16);
	string cusip = "912828M56";
	Bond treasuryBond(cusip, CUSIP, "T", 2.25, maturityDate);

	// Create 3 month USD LIOBR rate
	FloatingInterestRate interestRate("USDLIOBR3M", 3, LIBOR, 0.0);
  
	string f1_tBondMar20 = "T-Bond Mar20";
	date f1_maturityDate(2020, Mar, 1);
	BondFuture f1(f1_tBondMar20, treasuryBond, f1_maturityDate, 100000, 0.01, "ZB", "158-15");

	string f2_tBondJun20 = "T-Bond Jun20";
	date f2_maturityDate(2020, Jun, 1);
	BondFuture f2(f2_tBondJun20, treasuryBond, f2_maturityDate, 100000, 0.01, "ZB", "158-11");

	string f3_eurodollarMar20 = "Eurodollar Mar20";
	date f3_maturityDate(2020, Mar, 1);
	EuroDollarFuture f3(f3_eurodollarMar20, interestRate, f3_maturityDate, 1000000, 0.005, "GE", 98.12);

	// Create FutureProductService
	FutureProductService *futureProductService = new FutureProductService();

	// Add futures
	futureProductService->Add(f1);
	futureProductService->Add(f2);
	futureProductService->Add(f3);

	// retrieve products from serivce
	std::cout << "Future: " << futureProductService->GetData(f1_tBondMar20).GetProductId() << " == > " << f1_tBondMar20 << std::endl;
	std::cout << "Future: " << futureProductService->GetData(f2_tBondJun20).GetProductId() << " == > " << f2_tBondJun20 << std::endl;
	std::cout << "Future: " << futureProductService->GetData(f3_eurodollarMar20).GetProductId() << " == > " << f3_eurodollarMar20 << std::endl;
}

void printSwaps(vector<IRSwap> swaps)
{
	std::cout << "Found " << swaps.size() << " swaps\n";
}

void testSwapProductService()
{
	// Create the Spot 10Y Outright Swap
	date effectiveDate(2015, Nov, 16);
	date terminationDate(2025, Nov, 16);
	string outright10Y = "Spot-Outright-10Y";
	IRSwap outright10YSwap(outright10Y, THIRTY_THREE_SIXTY, THIRTY_THREE_SIXTY, SEMI_ANNUAL, LIBOR, TENOR_3M, effectiveDate, terminationDate, USD, 10, SPOT, OUTRIGHT);

	// Create the IMM 2Y Outright Swap
	date effectiveDate2(2015, Dec, 20);
	date terminationDate2(2017, Dec, 20);
	string imm2Y = "IMM-Outright-2Y";
	IRSwap imm2YSwap(imm2Y, THIRTY_THREE_SIXTY, THIRTY_THREE_SIXTY, SEMI_ANNUAL, LIBOR, TENOR_3M, effectiveDate2, terminationDate2, USD, 2, IMM, OUTRIGHT);

	// Create a IRSwapProductService
	IRSwapProductService *swapProductService = new IRSwapProductService();

	// Add the Spot 10Y Outright Swap to the IRSwapProductService and retrieve it from the service
	swapProductService->Add(outright10YSwap);
	swapProductService->Add(imm2YSwap);
	
	// Test swap service "GET" methods
	vector<IRSwap> swaps = swapProductService->GetSwaps(OUTRIGHT);
	std::cout << "Get swaps with OUTRIGHT swap leg type\n";
	printSwaps(swaps);

	swaps = swapProductService->GetSwaps(SPOT);
	std::cout << "Get swaps with SPOT swap leg type\n";
	printSwaps(swaps);

	swaps = swapProductService->GetSwapsLessThan(5);
	std::cout << "Get swaps less than 5 year tenor\n";
	printSwaps(swaps);

	swaps = swapProductService->GetSwaps(LIBOR);
	std::cout << "Get swaps with LIBOR floating index\n";
	printSwaps(swaps);

	swaps = swapProductService->GetSwaps(THIRTY_THREE_SIXTY);
	std::cout << "Get swaps with THIRTY_THREE_SIXTY day count convention\n";
	printSwaps(swaps);
}

void testBondProductService()
{
	// Create the 10Y treasury note
	date maturityDate(2025, Nov, 16);
	string cusip = "912828M56";
	Bond treasuryBond(cusip, CUSIP, "T", 2.25, maturityDate);

	// Create the 2Y treasury note
	date maturityDate2(2017, Nov, 5);
	string cusip2 = "912828TW0";
	Bond treasuryBond2(cusip2, CUSIP, "T", 0.75, maturityDate2);

	// Create a BondProductService
	BondProductService *bondProductService = new BondProductService();
	bondProductService->Add(treasuryBond);
	bondProductService->Add(treasuryBond2);
	
	// Test bond service "GET" methods
	string ticker = "T";
	std::cout << "Get bonds with ticker 'T' \n";
	std::cout << "Found " << bondProductService->GetBonds(ticker).size() << " bonds\n";

	ticker = "P";
	std::cout << "Get bonds with ticker 'P' \n";
	std::cout << "Found " << bondProductService->GetBonds(ticker).size() << " bonds\n";
}

int main()
{
	std::cout << "\n---- Test Future product Service ----\n";
	testFutureProductService();

	std::cout << "\n---- Test Swap product Service ----\n";
	testSwapProductService();

	std::cout << "\n---- Test Bond product Service ----\n";
	testBondProductService();

	std::cout << "\n----------- Press Any key to quit! -------------\n" << std::endl;
	std::cin.get();
	return 0;
}