#ifndef _LIBBASE64_H
#define _LIBBASE64_H

#ifdef __cplusplus
extern "C" {
#endif

/* These are the flags that can be passed in the `flags` argument. The values
 * below force the use of a given codec, even if that codec is a no-op in the
 * current build. Used in testing. Set to 0 for the default behavior, which is
 * runtime feature detection on x86, a compile-time fixed codec on ARM, and
 * the plain codec on other platforms: */
#define BASE64_FORCE_AVX2	(1 << 0)
#define BASE64_FORCE_NEON32	(1 << 1)
#define BASE64_FORCE_NEON64	(1 << 2)
#define BASE64_FORCE_PLAIN	(1 << 3)
#define BASE64_FORCE_SSSE3	(1 << 4)
#define BASE64_FORCE_SSE41	(1 << 5)
#define BASE64_FORCE_SSE42	(1 << 6)
#define BASE64_FORCE_AVX	(1 << 7)

struct base64_state {
	int eof;
	int bytes;
	int flags;
	unsigned char carry;
};

/* Wrapper function to encode a plain string of given length. Output is written
 * to *out without trailing zero. Output length in bytes is written to *outlen.
 * The buffer in `out` has been allocated by the caller and is at least 4/3 the
 * size of the input. See above for `flags`; set to 0 for default operation: */
void trk_base64_encode
	( const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	, int			 flags
	) ;

/* Call this before calling base64_stream_encode() to init the state. See above
 * for `flags`; set to 0 for default operation: */
void base64_stream_encode_init
	( struct base64_state	*state
	, int			 flags
	) ;

/* Encodes the block of data of given length at `src`, into the buffer at
 * `out`. Caller is responsible for allocating a large enough out-buffer; it
 * must be at least 4/3 the size of the in-buffer, but take some margin. Places
 * the number of new bytes written into `outlen` (which is set to zero when the
 * function starts). Does not zero-terminate or finalize the output. */
void base64_stream_encode
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	) ;

/* Finalizes the output begun by previous calls to `base64_stream_encode()`.
 * Adds the required end-of-stream markers if appropriate. `outlen` is modified
 * and will contain the number of new bytes written at `out` (which will quite
 * often be zero). */
void base64_stream_encode_final
	( struct base64_state	*state
	, char			*out
	, size_t		*outlen
	) ;

/* Wrapper function to decode a plain string of given length. Output is written
 * to *out without trailing zero. Output length in bytes is written to *outlen.
 * The buffer in `out` has been allocated by the caller and is at least 3/4 the
 * size of the input. See above for `flags`, set to 0 for default operation: */
int trk_base64_decode
	( const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	, int			 flags
	) ;

/* Call this before calling base64_stream_decode() to init the state. See above
 * for `flags`; set to 0 for default operation: */
void base64_stream_decode_init
	( struct base64_state	*state
	, int			 flags
	) ;

/* Decodes the block of data of given length at `src`, into the buffer at
 * `out`. Caller is responsible for allocating a large enough out-buffer; it
 * must be at least 3/4 the size of the in-buffer, but take some margin. Places
 * the number of new bytes written into `outlen` (which is set to zero when the
 * function starts). Does not zero-terminate the output. Returns 1 if all is
 * well, and 0 if a decoding error was found, such as an invalid character.
 * Returns -1 if the chosen codec is not included in the current build. Used by
 * the test harness to check whether a codec is available for testing. */
int base64_stream_decode
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	) ;

#ifdef __cplusplus
}
#endif

#endif /* _LIBBASE64_H */
