#ifndef BASE_PROFILE_H
#define BASE_PROFILE_H

///////////////////////////////////////
/// cjk: Profiling Definitions

static clock_t global_profiling_begin, global_profiling_end;

#define ProfileFunc() 	Profile(__func__)
#define Profile(description)	DeferLoop(profile_begin(), profile_end(Str8Lit(description)))

void profile_begin();
void profile_end(Str8 description);


#endif // BASE_PROFILE_H
