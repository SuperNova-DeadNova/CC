#ifndef CC_RESOURCES_H
#define CC_RESOURCES_H
#include "Core.h"
/* Implements checking, fetching, and patching the default game assets.
	Copyright 2014-2019 ClassiCube | Licensed under BSD-3
*/

/* Number of resources that need to be downloaded. */
extern int Resources_Count;
/* Total size of resources that need to be downloaded. */
extern int Resources_Size;
/* Checks existence of all assets. */
void Resources_CheckExistence(void);

/* Whether fetcher is currently downloading resources. */
extern bool Fetcher_Working;
/* Whether fetcher has finished. (downloaded all resources, or an error) */
extern bool Fetcher_Completed;
/* Number of resources that have been downloaded so far. */
extern int Fetcher_Downloaded;
/* HTTP status code of last failed resource download */
extern int Fetcher_StatusCode;
/* Error code of last failed resource download. */
extern ReturnCode Fetcher_Result;
/* Whether a resource failed to download. */
extern bool Fetcher_Failed;

/* Starts asynchronous download of missing resources. */
void Fetcher_Run(void);
/* Checks if any resources have finished downloading. */
/* If any have, performs required patching and saving. */
void Fetcher_Update(void);
#endif
