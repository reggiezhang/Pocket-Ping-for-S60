/*
 * util.h
 *
 *  Created on: Dec 23, 2008
 *      Author: ezhenzh
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <eikbtgpc.h>
namespace Util
{
	inline void SetCbaL(TInt aResId)
	{
		if (CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current())
		{
			cba->SetCommandSetL(aResId);
			cba->DrawNow();
		}
	}
}

#endif /* UTIL_H_ */
