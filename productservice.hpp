/**
* productservice.hpp defines Bond and IRSwap ProductServices
*/

#include <iostream>
#include <functional>
#include <map>
#include "products.hpp"
#include "soa.hpp"

/**
* Bond Product Service to own reference data over a set of bond securities.
* Key is the productId string, value is a Bond.
*/
class BondProductService : public Service<string, Bond>
{

public:
	// BondProductService ctor
	BondProductService();

	// Return the bond data for a particular bond product identifier
	Bond& GetData(string productId);

	// Add a bond to the service (convenience method)
	void Add(Bond &bond);

	// Get all Bonds with the specified ticker
	vector<Bond> GetBonds(string& _ticker);

private:
	map<string, Bond> bondMap; // cache of bond products

};

/**
* Interest Rate Swap Product Service to own reference data over a set of IR Swap products
* Key is the productId string, value is a IRSwap.
*/
class IRSwapProductService : public Service<string, IRSwap>
{
public:
	// IRSwapProductService ctor
	IRSwapProductService();

	// Return the IR Swap data for a particular bond product identifier
	IRSwap& GetData(string productId);

	// Add a bond to the service (convenience method)
	void Add(IRSwap &swap);

	// Get all Swaps with the specified fixed leg day count convention
	vector<IRSwap> GetSwaps(DayCountConvention _fixedLegDayCountConvention);

	// Get all Swaps with the specified fixed leg payment frequency
	vector<IRSwap> GetSwaps(PaymentFrequency _fixedLegPaymentFrequency);

	// Get all Swaps with the specified floating index
	vector<IRSwap> GetSwaps(FloatingIndex _floatingIndex);

	// Get all Swaps with a term in years greater than the specified value
	vector<IRSwap> GetSwapsGreaterThan(int _termYears);

	// Get all Swaps with a term in years less than the specified value
	vector<IRSwap> GetSwapsLessThan(int _termYears);

	// Get all Swaps with the specified swap type
	vector<IRSwap> GetSwaps(SwapType _swapType);

	// Get all Swaps with the specified swap leg type
	vector<IRSwap> GetSwaps(SwapLegType _swapLegType);

private:
	map<string, IRSwap> swapMap; // cache of IR Swap products

	vector<IRSwap> GetSwaps(std::function<bool(IRSwap)> filterFunc)
	{
		std::vector<IRSwap> swaps;
		// show content:
		for (std::map<string, IRSwap>::iterator it = swapMap.begin(); it != swapMap.end(); ++it)
			if (filterFunc(it->second))
				swaps.push_back(it->second);

		return swaps;
	}
};

/*---------------------- Bond Service start ---------------------*/
BondProductService::BondProductService()
{
	bondMap = map<string, Bond>();
}

Bond& BondProductService::GetData(string productId)
{
	return bondMap[productId];
}

void BondProductService::Add(Bond &bond)
{
	bondMap.insert(pair<string, Bond>(bond.GetProductId(), bond));
}

vector<Bond> BondProductService::GetBonds(string& _ticker)
{
	std::vector<Bond> bonds;
	for (std::map<string, Bond>::iterator it = bondMap.begin(); it != bondMap.end(); ++it)
		if (it->second.GetTicker() == _ticker)
			bonds.push_back(it->second);

	return bonds;
}
/*--------------------- Bond Service End --------------------------*/

/*--------------------- IR SWAP Service start --------------------- */
IRSwapProductService::IRSwapProductService()
{
	swapMap = map<string, IRSwap>();
}

IRSwap& IRSwapProductService::GetData(string productId)
{
	return swapMap[productId];
}

void IRSwapProductService::Add(IRSwap &swap)
{
	swapMap.insert(pair<string, IRSwap>(swap.GetProductId(), swap));
}

vector<IRSwap> IRSwapProductService::GetSwaps(DayCountConvention _fixedLegDayCountConvention)
{
	std::function<bool(IRSwap)> func = [_fixedLegDayCountConvention](IRSwap s)->bool { return s.GetFixedLegDayCountConvention() == _fixedLegDayCountConvention; };
	return GetSwaps(func);
}

vector<IRSwap> IRSwapProductService::GetSwaps(PaymentFrequency _fixedLegPaymentFrequency) 
{
	std::function<bool(IRSwap)> func = [_fixedLegPaymentFrequency](IRSwap s)->bool { return s.GetFixedLegPaymentFrequency() == _fixedLegPaymentFrequency; };
	return GetSwaps(func);
}

vector<IRSwap> IRSwapProductService::GetSwaps(FloatingIndex _floatingIndex)
{
	std::function<bool(IRSwap)> func = [_floatingIndex](IRSwap s)->bool { return s.GetFloatingIndex() == _floatingIndex; };
	return GetSwaps(func);
}

vector<IRSwap> IRSwapProductService::GetSwapsGreaterThan(int _termYears)
{
	std::function<bool(IRSwap)> func = [_termYears](IRSwap s)->bool { return s.GetTermYears() >= _termYears; };
	return GetSwaps(func);
}

vector<IRSwap> IRSwapProductService::GetSwapsLessThan(int _termYears)
{
	std::function<bool(IRSwap)> func = [_termYears](IRSwap s)->bool { return s.GetTermYears() < _termYears; };
	return GetSwaps(func);
}

vector<IRSwap> IRSwapProductService::GetSwaps(SwapType _swapType)
{
	std::function<bool(IRSwap)> func = [_swapType](IRSwap s)->bool { return s.GetSwapType() == _swapType; };
	return GetSwaps(func);
}

vector<IRSwap> IRSwapProductService::GetSwaps(SwapLegType _swapLegType)
{
	std::function<bool(IRSwap)> func = [_swapLegType](IRSwap s)->bool { return s.GetSwapLegType() == _swapLegType; };
	return GetSwaps(func);
}

/*--------------------- IR SWAP Service end --------------------- */

/**
* Future Product Service to own reference data over a set of futures.
* Key is the productId string, value is a Bond.
*/
class FutureProductService : public Service<string, Future>
{
public:
	FutureProductService() { futureMap = map<string, Future>(); };
	void Add(Future future) { futureMap.insert(pair<string, Future>(future.GetProductId(), future)); }

	Future& GetData(string productId) { return futureMap[productId]; }
protected:
	map<string, Future> futureMap; // cache product
};
