/* Copyright (C) 2024 Streanga Sarmis-Stefan - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the CC0 license. Which can be found
 * here: https://creativecommons.org/public-domain/cc0/
 *
 * JII aims to be a set of libraries really easy to use and link.
 * It should take away the headache C++ libraries tend to give nowadays
 * with self building libraries that don't build out of the box,
 * have weird dependencies and waste precious time that could 
 * be spent doing actual work.
 * 
 * #define JII_OBJ_IMPLMENTATION in some C/C++ file to compile the functions 
 * so the symbols can be found
 * 
 * Don't define JII_OBJ_IMPLMENTATION in more than one file because
 * there will be duplicated symbols, I am pretty sure people don't even
 * read these comments on the top of the header but a man can hope.
 * 
 * JIIObjModelData model;
 * JIIObjStatus status = JIIObjLoadData("path/to/obj", &model);
 * 
 * ...do stuff with it...
 * 
 * JIIObjFreeData(&model);
 */

#pragma once

#include <stdio.h>

#ifndef JII_PRIMITIVE_DEFINES
#define JII_PRIMITIVE_DEFINES
#include <stdint.h>
typedef int32_t i32;
typedef int8_t i8;
typedef uint32_t u32;
typedef uint8_t u8;
#endif

struct JIIObjPosition {
	float x;
	float y;
	float z;
};

struct JIIObjUV {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		struct {
			float u;
			float v;
			float w;
		};
	};
};

struct JIIObjNormal {
	float x;
	float y;
	float z;
};

struct JIIObjVertex {
	JIIObjPosition position;
	JIIObjUV uv;
	JIIObjNormal normal;
};

// TODO(Sarmis) expand to non-triangulated faces
struct JIIObjFace {
	union {
		struct {
			i32 index0;
			i32 index1;
			i32 index2;
		};
		i32 indices[3];
	};
};

struct JIIObjModelData {
	JIIObjPosition* positions;
	i32 numberOfPositions;

	JIIObjNormal* normals;
	i32 numberOfNormals;

	JIIObjUV* uvs;
	i32 numberOfUVs;

	JIIObjFace* faces;
	i32 numberOfFaces;

	JIIObjVertex* vertices;
	i32 numberOfVertices;
};

enum JIIObjStatus {
	Ok,
	Error,
	Eof,
	OutOfSpace
};

#ifdef __cplusplus
extern "C" {
#endif

#ifndef JIIDef
#ifdef JII_OBJ_STATIC
#define JIIDef static
#else
#define JIIDef extern
#endif
#endif

JIIDef JIIObjStatus JIIObjLoadData(const char* path, JIIObjModelData* data);
JIIDef JIIObjStatus JIIObjLoadDataW(const wchar_t* path, JIIObjModelData* data);

JIIDef void JIIObjFreeData(JIIObjModelData* data);

#ifdef __cplusplus
}
#endif

#ifdef JII_OBJ_IMPLMENTATION

#ifndef JIIPrivate
#define JIIPrivate static
#endif

#ifndef JIIAssert
#include <assert.h>
#define JIIAssert(...) assert(__VA_ARGS__)
#endif

#ifndef JIIMalloc
#include <stdlib.h>
#define JIIMalloc(...) malloc(__VA_ARGS__)
#endif

#ifndef JIIFree
#include <stdlib.h>
#define JIIFree(...) free(__VA_ARGS__)
#endif

struct JIIObjContext {
	// for parsing
	u8* fileBuffer;
	u32 fileSize;
	u32 fileCursor;

	// for indices
	u32 usedPositions;
	u32 usedNormals;
	u32 usedUVs;
	u32 usedVertices;
	u32 usedFaces;

	// for multiple objects
	u32 positionsOffset;
	u32 uvsOffset;
	u32 normalsOffset;

	// the output
	JIIObjModelData modelData;
};

JIIPrivate JIIObjStatus JIIObjCopyFileContentsToMemory(FILE* file, u8** buffer, u32* size) {
	JIIAssert(file && size && buffer);

	fseek(file, 0, SEEK_END);
	*size = ftell(file);
	fseek(file, 0, SEEK_SET);

	*buffer = (u8*)JIIMalloc(*size);

	i32 bytesRead = fread(*buffer, 1, *size, file);
	while (bytesRead != *size) {
		bytesRead += fread(*buffer + bytesRead, 1, *size - bytesRead, file);
	}

	return JIIObjStatus::Ok;
}

JIIPrivate JIIObjStatus JIIObjReadFile(const char* path, u8** buffer, u32* size) {
	JIIAssert(path && size && buffer);
	
	FILE* file;

	if (fopen_s(&file, path, "rb") != 0) {
		*buffer = NULL;
		*size = 0;
		return JIIObjStatus::Error;
	}

	JIIObjStatus result = JIIObjCopyFileContentsToMemory(file, buffer, size);

	fclose(file);

	return result;
}

JIIPrivate JIIObjStatus JIIObjReadFileW(const wchar_t* path, u8** buffer, u32* size) {
	JIIAssert(path && size && buffer);
	
	FILE* file;

	if (_wfopen_s(&file, path, L"rb") != 0) {
		*buffer = NULL;
		*size = 0;
		return JIIObjStatus::Error;
	}

	JIIObjStatus result = JIIObjCopyFileContentsToMemory(file, buffer, size);

	fclose(file);

	return result;
}

JIIPrivate bool JIIObjIsDigit(char c) {
	return c >= '0' && c <= '9';
}

JIIPrivate bool JIIObjIsWhitespace(char c) {
	return (c == ' ' || c == '\t');
}

JIIPrivate bool JIIObjIsLineEnd(char c) {
	return (c == '\n' || c == '\r');
}

JIIPrivate JIIObjStatus JIIObjEatLineEnd(JIIObjContext* context) {
	JIIAssert(context);

	while (context->fileBuffer[context->fileCursor] == '\n' ||
		context->fileBuffer[context->fileCursor] == '\r') {
		++context->fileCursor;
		if (context->fileCursor >= context->fileSize) {
			return JIIObjStatus::Eof;
		}
	}

	return JIIObjStatus::Ok;
}

JIIPrivate JIIObjStatus JIIObjSkipToNextLine(JIIObjContext* context) {
	JIIAssert(context);

	while (!JIIObjIsLineEnd(context->fileBuffer[context->fileCursor])) {
		++context->fileCursor;
		if (context->fileCursor >= context->fileSize) {
			return JIIObjStatus::Eof;
		}
	}
	
	return JIIObjEatLineEnd(context);
}

JIIPrivate JIIObjStatus JIIObjReadLine(JIIObjContext* context, u8* lineBuffer, u32* size, u32 maxSize) {
	JIIAssert(context);

	// # is a comment, just skip the line
	if (context->fileBuffer[context->fileCursor] == '#') {
		return JIIObjSkipToNextLine(context);
	}

	u32 offset = 0;
	while (!JIIObjIsLineEnd(context->fileBuffer[context->fileCursor])) {
		if (offset >= maxSize) {
			return JIIObjStatus::OutOfSpace;
		}

		lineBuffer[offset++] = context->fileBuffer[context->fileCursor];
		++context->fileCursor;
		if (context->fileCursor >= context->fileSize) {
			return JIIObjStatus::Eof;
		}
	}

	*size = offset;

	return JIIObjEatLineEnd(context);
}

JIIPrivate void JIIObjEatWhitespaces(u8* lineBuffer, u32 lineSize, u32* offset) {
	JIIAssert(lineBuffer);

	while (JIIObjIsWhitespace(lineBuffer[*offset])) {
		++(*offset);
		if (*offset >= lineSize) {
			return;
		}
	}
}

#define JII_ADVANCE_CHECK_RETURN(offset, size, returnValue) {\
	++(offset);\
	if ((offset) >= (size)) {\
		return (returnValue);\
	}\
}
#define JII_ADVANCE_CHECK_RETURN_NOVALUE(offset, size) {\
	++(offset);\
	if ((offset) >= (size)) {\
		return;\
	}\
}
#define JII_ADVANCE_CHECK_BREAK(offset, size) {\
	++(offset);\
	if ((offset) >= (size)) {\
		break;\
	}\
}

JIIPrivate u32 JIIObjEatU32(u8* lineBuffer, u32 lineSize, u32* offset) {
	u32 result = 0;

	JIIObjEatWhitespaces(lineBuffer, lineSize, offset);

	while (JIIObjIsDigit(lineBuffer[*offset])) {
		result = result * 10 + (lineBuffer[*offset] - '0');
		JII_ADVANCE_CHECK_RETURN(*offset, lineSize, result);
	}

	return result;
}

JIIPrivate float JIIObjEatFloat(u8* lineBuffer, u32 lineSize, u32* offset) {
	JIIAssert(lineBuffer);

	JIIObjEatWhitespaces(lineBuffer, lineSize, offset);

	float result = 0;
	float power = 1.0;

	bool isFractionalPart = false;

	i8 sign = 1;

	switch(lineBuffer[*offset]) {
		case '-': {
			sign = -1;
			++(*offset);
			break;
		}
		case '+': {
			++(*offset);
			break;
		}
		default: {
			break;
		}
	}

	if (*offset >= lineSize) {
		return sign * (result / power);
	}

	while (JIIObjIsDigit(lineBuffer[*offset])) {
		result = result * 10 + (lineBuffer[*offset] - '0');
		
		JII_ADVANCE_CHECK_RETURN(*offset, lineSize, sign * (result / power));
	}

	if (lineBuffer[*offset] == '.') {
		JII_ADVANCE_CHECK_RETURN(*offset, lineSize, sign * (result / power));
	}

	while (JIIObjIsDigit(lineBuffer[*offset])) {
		result = result * 10 + (lineBuffer[*offset] - '0');
		power *= 10;

		JII_ADVANCE_CHECK_RETURN(*offset, lineSize, sign * (result / power));
	}

	if (lineBuffer[*offset] == 'e' || lineBuffer[*offset] == 'E') {
		JII_ADVANCE_CHECK_RETURN(*offset, lineSize, sign * (result / power));
	}
	else {
		return sign * (result / power);
	}

	i8 eSign = 1;
	switch (lineBuffer[*offset]) {
		case '-': {
			eSign = -1;
			++(*offset);
			break;
		}
		case '+': {
			++(*offset);
			break;
		}
		default: {
			break;
		}
	}

	if (*offset >= lineSize) {
		return sign * (result / power);
	}

	i32 power2 = 0;
	while (JIIObjIsDigit(lineBuffer[*offset])) {
		power2 = power2 * 10 + (lineBuffer[*offset] - '0');

		JII_ADVANCE_CHECK_RETURN(*offset, lineSize, sign * (result / power));
	}

	while (power2 != 0) {
		power *= (eSign == 1) ? 0.1 : 10;
		--power2;
	}

	return sign * (result / power);
}

// this function is the definition of spray and pray
JIIPrivate JIIObjStatus JIIObjParseVertexAttribute(JIIObjContext* context, u8* lineBuffer, u32 lineSize, u32 offset) {
	JIIAssert(context && lineBuffer && lineSize);

	if (offset + 1 >= lineSize) {
		return JIIObjStatus::Eof;
	}

	// function is always called after 'v' was already detected
	++offset;
	switch (lineBuffer[offset]) {
		case ' ': {
			// going to be position
			++offset;
			float x = JIIObjEatFloat(lineBuffer, lineSize, &offset);
			float y = JIIObjEatFloat(lineBuffer, lineSize, &offset);
			float z = JIIObjEatFloat(lineBuffer, lineSize, &offset);
			context->modelData.positions[context->usedPositions++] = {x, y, z};
			break;
		}

		case 't': {
			// going to be uv
			++offset;
			float u = JIIObjEatFloat(lineBuffer, lineSize, &offset);
			float v = JIIObjEatFloat(lineBuffer, lineSize, &offset);
			float w = JIIObjEatFloat(lineBuffer, lineSize, &offset);
			context->modelData.uvs[context->usedUVs++] = { u, v, w };
			break;
		}

		case 'n': {
			// going to be normal
			++offset;
			float x = JIIObjEatFloat(lineBuffer, lineSize, &offset);
			float y = JIIObjEatFloat(lineBuffer, lineSize, &offset);
			float z = JIIObjEatFloat(lineBuffer, lineSize, &offset);
			context->modelData.normals[context->usedNormals++] = { x, y, z };
			break;
		}
	}

	return JIIObjStatus::Ok;
}

JIIPrivate JIIObjStatus JIIObjParseFace(JIIObjContext* context, u8* lineBuffer, u32 lineSize, u32 offset) {
	JIIAssert(context && lineBuffer && lineSize);
	JII_ADVANCE_CHECK_RETURN(offset, lineSize, JIIObjStatus::Eof);

	u32 verticesInFace = 0;

	u32 cachedIndex0;
	u32 cachedIndex1;

	while (JIIObjIsWhitespace(lineBuffer[offset])) {
		JII_ADVANCE_CHECK_RETURN(offset, lineSize, JIIObjStatus::Eof);

		u32 position = UINT_MAX;
		u32 uv = UINT_MAX;
		u32 normal = UINT_MAX;

		position = JIIObjEatU32(lineBuffer, lineSize, &offset);
		if (offset < lineSize && lineBuffer[offset] == '/') {
			JII_ADVANCE_CHECK_RETURN(offset, lineSize, JIIObjStatus::Eof);
			// f p//n p//n p//n is accepted input
			if (lineBuffer[offset] != '/') {
				uv = JIIObjEatU32(lineBuffer, lineSize, &offset);
			}

			if (offset < lineSize && lineBuffer[offset] == '/') {
				JII_ADVANCE_CHECK_RETURN(offset, lineSize, JIIObjStatus::Eof);
				normal = JIIObjEatU32(lineBuffer, lineSize, &offset);
			}
		}

		JIIObjVertex vertex = {};

		vertex.position = context->modelData.positions[position + context->positionsOffset];
		if (uv != UINT_MAX) {
			vertex.uv = context->modelData.uvs[uv + context->uvsOffset];
		}
		if (normal != UINT_MAX) {
			vertex.normal = context->modelData.normals[normal + context->normalsOffset];
		}

		context->modelData.vertices[context->usedVertices++] = vertex;

		++verticesInFace;
		if (verticesInFace == 3) {
			JIIObjFace face = {};
			face.indices[0] = context->usedVertices - 2;
			face.indices[1] = context->usedVertices - 1;
			face.indices[2] = context->usedVertices;

			context->modelData.faces[context->usedFaces++] = face;

			cachedIndex0 = face.indices[0];
			cachedIndex1 = face.indices[2];
		}
		else if(verticesInFace > 3){
			// triangulate face
			JIIObjFace face = {};
			face.indices[0] = cachedIndex0;
			face.indices[1] = cachedIndex1;
			face.indices[2] = context->usedVertices;

			context->modelData.faces[context->usedFaces++] = face;

			cachedIndex1 = face.indices[2];
		}
	}
}

JIIPrivate JIIObjStatus JIIObjParseLine(JIIObjContext* context, u8* lineBuffer, u32 lineSize) {
	JIIAssert(context && lineBuffer && lineSize);

	JIIObjStatus status = JIIObjStatus::Ok;

	u32 offset = 0;
	switch (lineBuffer[offset]) {
		case 'v': {
			status = JIIObjParseVertexAttribute(context, lineBuffer, lineSize, offset);
			break;
		}
		case 'f': {
			status = JIIObjParseFace(context, lineBuffer, lineSize, offset);
			break;
		}
		default: {
			break;
		}
		// not caring about other attributes for now
	}

	return status;
}

JIIPrivate void JIIObjTryPeekAttribute(JIIObjContext* context) {
	JIIAssert(context);

	switch (context->fileBuffer[context->fileCursor]) {
		case 'v': {
			if (context->fileCursor + 1 < context->fileSize) {
				switch (context->fileBuffer[context->fileCursor + 1]) {
					case ' ': {
						++context->modelData.numberOfPositions;
						break;
					}
					case 't': {
						++context->modelData.numberOfUVs;
						break;
					}
					case 'n': {
						++context->modelData.numberOfNormals;
						break;
					}
				}
			}
			break;
		}
		// this is more fragile than my ego
		case 'f': {
			// skip whitespace right after
			JII_ADVANCE_CHECK_RETURN_NOVALUE(context->fileCursor, context->fileSize);
			JII_ADVANCE_CHECK_RETURN_NOVALUE(context->fileCursor, context->fileSize);

			u32 spaces = 0;
			while (!JIIObjIsLineEnd(context->fileBuffer[context->fileCursor])) {
				if (JIIObjIsWhitespace(context->fileBuffer[context->fileCursor])) {
					++spaces;
				}
				JII_ADVANCE_CHECK_BREAK(context->fileCursor, context->fileSize);
			}

			u32 verticesPerFace = spaces + 1;
			
			JIIAssert(verticesPerFace == 3 || verticesPerFace > 3);

			if (verticesPerFace > 3) {
				// when we triangulate faces we will get more faces from non-triangulated face
				context->modelData.numberOfFaces += verticesPerFace - 2;
			}
			else if(verticesPerFace == 3){
				++context->modelData.numberOfFaces;
			}
			
			break;
		}
	}
}

JIIPrivate JIIObjStatus JIIObjPeekFile(JIIObjContext* context) {
	JIIAssert(context);

	u32 saveCursor = context->fileCursor;

	JIIObjStatus status;
	while (true) {
		JIIObjTryPeekAttribute(context);
		status = JIIObjSkipToNextLine(context);
		if (status != JIIObjStatus::Ok) {
			break;
		}
	}

	context->fileCursor = saveCursor;

	return JIIObjStatus::Ok;
}

JIIPrivate JIIObjStatus JIIObjParseBuffer(JIIObjContext* context) {
	JIIAssert(context);

	// peek in order to preallocate all the needed space
	JIIObjPeekFile(context);
	// TODO(Sarmis) cache vertices
	context->modelData.numberOfVertices = context->modelData.numberOfFaces * 3;
	context->modelData.positions = (JIIObjPosition*)JIIMalloc(sizeof(JIIObjPosition) * context->modelData.numberOfPositions);
	context->modelData.normals = (JIIObjNormal*)JIIMalloc(sizeof(JIIObjNormal) * context->modelData.numberOfNormals);
	context->modelData.uvs = (JIIObjUV*)JIIMalloc(sizeof(JIIObjUV) * context->modelData.numberOfUVs);
	context->modelData.faces = (JIIObjFace*)JIIMalloc(sizeof(JIIObjFace) * context->modelData.numberOfFaces);
	context->modelData.vertices = (JIIObjVertex*)JIIMalloc(sizeof(JIIObjVertex) * context->modelData.numberOfVertices);
	
	u8 lineBuffer[256];
	u32 lineSize;
	JIIObjStatus status;
	while (true) {
		status = JIIObjReadLine(context, lineBuffer, &lineSize, 256);
		if (status != JIIObjStatus::Ok) {
			return status;
		}

		if (lineSize != 0) {
			status = JIIObjParseLine(context, lineBuffer, lineSize);
			if (status != JIIObjStatus::Ok) {
				return status;
			}
		}
	}
}

JIIDef JIIObjStatus JIIObjLoadData(const char* path, JIIObjModelData* data) {
	JIIAssert(path && data);

	JIIObjContext context = {};

	JIIObjStatus status;

	status = JIIObjReadFile(path, &context.fileBuffer, &context.fileSize);
	if (status != JIIObjStatus::Ok) {
		return status;
	}

	status = JIIObjParseBuffer(&context);
	if (status != JIIObjStatus::Ok && status != JIIObjStatus::Eof) {
		return status;
	}

	JIIFree(context.fileBuffer);

	*data = context.modelData;

	return JIIObjStatus::Ok;
}

JIIDef JIIObjStatus JIIObjLoadDataW(const wchar_t* path, JIIObjModelData* data) {
	JIIAssert(path && data);

	JIIObjContext context = {};

	JIIObjStatus status;

	status = JIIObjReadFileW(path, &context.fileBuffer, &context.fileSize);
	if (status != JIIObjStatus::Ok) {
		return status;
	}

	status = JIIObjParseBuffer(&context);
	if (status != JIIObjStatus::Ok && status != JIIObjStatus::Eof) {
		return status;
	}

	JIIFree(context.fileBuffer);

	*data = context.modelData;
	return JIIObjStatus::Ok;
}

JIIDef void JIIObjFreeData(JIIObjModelData* data) {
	JIIAssert(data);

	JIIFree(data->positions);
	JIIFree(data->uvs);
	JIIFree(data->normals);
	JIIFree(data->faces);
	JIIFree(data->vertices);
}

#endif // JII_OBJ_IMPLMENTATION
