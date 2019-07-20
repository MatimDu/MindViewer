/* 确保此头文件只被包含一次 */
#ifndef THINKGEARSTREAMPARSER_HPP
#define THINKGEARSTREAMPARSER_HPP

/* 包含所有所需外部库*/
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

/*当编译CPP时定义外部C类型*/
#ifdef __cplusplus
extern "C" {
#endif

/* 解析类型*/
#define PARSER_TYPE_NULL       0x00
#define PARSER_TYPE_PACKETS    0x01    /* TGAM流数据*/
#define PARSER_TYPE_2BYTERAW   0x02    /* 2位的源数据*/

/* 定义数据代码*/
#define PARSER_CODE_BATTERY            0x01
#define PARSER_CODE_POOR_QUALITY       0x02 //信号质量
#define PARSER_CODE_ATTENTION          0x04 //注意度
#define PARSER_CODE_MEDITATION         0x05 //冥想值
#define PARSER_CODE_8BITRAW_SIGNAL     0x06 //8位源信号
#define PARSER_CODE_RAW_MARKER         0x07 //源标志

#define PARSER_CODE_RAW_SIGNAL         0x80 //源信号
#define PARSER_CODE_EEG_POWERS         0x81
#define PARSER_CODE_ASIC_EEG_POWER_INT 0x83

/* 解码状态（包）*/
#define PARSER_STATE_NULL           0x00  /* 未定义状态*/
#define PARSER_STATE_SYNC           0x01  /* 等待同步位*/
#define PARSER_STATE_SYNC_CHECK     0x02  /* Waiting for second SYNC byte */
#define PARSER_STATE_PAYLOAD_LENGTH 0x03  /* Waiting for payload[] length */
#define PARSER_STATE_PAYLOAD        0x04  /* Waiting for next payload[] byte */
#define PARSER_STATE_CHKSUM         0x05  /* Waiting for chksum byte */

/* 解码状态 (2位源数据解码) */
#define PARSER_STATE_WAIT_HIGH      0x06  /* Waiting for high byte */
#define PARSER_STATE_WAIT_LOW       0x07  /* High r'cvd.  Expecting low part */

/*其他常量*/
#define PARSER_SYNC_BYTE            0xAA  /* Syncronization byte */
#define PARSER_EXCODE_BYTE          0x55  /* EXtended CODE level byte */

/**
 * 解析器是一个管理解析状态的状态机
 */
typedef struct _ThinkGearStreamParser {

    unsigned char   type;
    unsigned char   state;

    unsigned char   lastByte;

    unsigned char   payloadLength;
    unsigned char   payloadBytesReceived;
    unsigned char   payload[256];
    unsigned char   payloadSum;
    unsigned char   chksum;//检验值

    void (*handleDataValue)( unsigned char extendedCodeLevel,
                             unsigned char code, unsigned char numBytes,
                             const unsigned char *value, void *customData );
    void  *customData;

} ThinkGearStreamParser;

/* 声明私有功能接口*/
static int
parsePacketPayload( ThinkGearStreamParser *parser );
static int
parseDataRow( ThinkGearStreamParser *parser, unsigned char *rowPtr );


/**
 * @param parser              Pointer to a ThinkGearStreamParser object.
 * @param parserType          One of the PARSER_TYPE_* constants defined above:
 *                            PARSER_TYPE_PACKETS or PARSER_TYPE_2BYTERAW.
 * @param handleDataValueFunc A user-defined callback function that will
 *                            be called whenever a data value is parsed
 *                            from a Packet.
 * @param customData          A pointer to any arbitrary data that will
 *                            also be passed to the handleDataValueFunc
 *                            whenever a data value is parsed from a
 *                            Packet.
 *
 * @return -1 if @c parser is NULL.
 * @return -2 if @c parserType is invalid.
 * @return 0 成功.
 */

/**
 * @param parser              指向 ThinkGearStreamParser 对象的指针.
 * @param parserType          水面已定义的PARSER_TYPE_* constants 之一:
 *                            PARSER_TYPE_PACKETS or PARSER_TYPE_2BYTERAW.
 * @param handleDataValueFunc A user-defined callback function that will
 *                            be called whenever a data value is parsed
 *                            from a Packet.
 * @param customData          A pointer to any arbitrary data that will
 *                            also be passed to the handleDataValueFunc
 *                            whenever a data value is parsed from a
 *                            Packet.
 *
 * @return -1 if @c parser 为空.
 * @return -2 if @c parserType 无效.
 * @return 0 成功.
 */
static int
THINKGEAR_initParser( ThinkGearStreamParser *parser, unsigned char parserType,
                      void (*handleDataValueFunc)(
                          unsigned char extendedCodeLevel,
                          unsigned char code, unsigned char numBytes,
                          const unsigned char *value, void *customData),
                      void *customData )
{

    if( !parser ) return( -1 );

    /* Initialize the parser's state based on the parser type */
    switch( parserType ) {
        case( PARSER_TYPE_PACKETS ):
            parser->state = PARSER_STATE_SYNC;
            break;
        case( PARSER_TYPE_2BYTERAW ):
            parser->state = PARSER_STATE_WAIT_HIGH;
            break;
        default: return( -2 );
    }

    /* Save parser type */
    parser->type = parserType;

    /* Save user-defined handler function and data pointer */
    parser->handleDataValue = handleDataValueFunc;
    parser->customData = customData;

    return( 0 );
}

/**
 * This is merely an example function prototype for a handleDataValueFunc()
 * callback function to be passed to THINKGEAR_initParser().  The user is
 * expected to actually define a function with this function signature, and pass
 * its name into the THINKGEAR_initParser() to create a ThinkGearStreamParser.
 * Then, each time a Data Value is successfully parsed by THINKGEAR_parseByte(),
 * this function gets automatically called, so that the user's application can
 * do whatever it likes with the @c value.
 *
 * The @c extendedCodeLevel and @c code together tells the application
 * what type of Data Value was received. The @c numBytes indicates the
 * number of bytes in the @c value[] array.
 *
 * The @c customData is a pointer to whatever user-defined data was
 * originally passed to THINKGEAR_initParser().
 *
 */
static void
exampleHandleDataValueFunc( unsigned char extendedCodeLevel,
                            unsigned char code,
                            unsigned char numBytes,
                            const unsigned char *value,
                            void *customData );


/**
 * Feeds the @c byte into the @c parser.  If the @c byte completes a
 * complete, valid parser, then the @c parser's handleDataValue()
 * function is automatically called on each DataRow in the Packet.
 * The return value provides an indication of the state of the
 * @c parser after parsing the byte.
 *
 * @param parser Pointer to an initialized ThinkGearDataParser object.
 * @param byte   The next byte of the data stream.
 *
 * @return -1 if @c parser is NULL.
 * @return -2 if a complete Packet was received, but the checksum failed.
 * @return -3 if an invalid Packet with PLENGTH > 170 was detected.
 * @return -4 if an invalid Packet with PLENGTH == 170 was detected.
 * @return -5 if the @c parser is somehow in an unrecognized state.
 * @return 0 if the @c byte did not yet complete a Packet.
 * @return 1 if a Packet was received and parsed successfully.
 *
 */
static int
THINKGEAR_parseByte( ThinkGearStreamParser *parser, unsigned char byte )
{

    int returnValue = 0;

    if( !parser ) return( -1 );

    /* Pick handling according to current state... */
    switch( parser->state ) {

        /* Waiting for SyncByte */
        case( PARSER_STATE_SYNC ):
            if( byte == PARSER_SYNC_BYTE ) {
                parser->state = PARSER_STATE_SYNC_CHECK;
            }
            break;

        /* Waiting for second SyncByte */
        case( PARSER_STATE_SYNC_CHECK ):
            if( byte == PARSER_SYNC_BYTE ) {
                parser->state = PARSER_STATE_PAYLOAD_LENGTH;
            } else {
                parser->state = PARSER_STATE_SYNC;
            }
            break;

        /* Waiting for Data[] length */
        case( PARSER_STATE_PAYLOAD_LENGTH ):
            parser->payloadLength = byte;
            if( parser->payloadLength > 170 ) {
                parser->state = PARSER_STATE_SYNC;
                returnValue = -3;
            } else if( parser->payloadLength == 170 ) {
                returnValue = -4;
            } else {
                parser->payloadBytesReceived = 0;
                parser->payloadSum = 0;
                parser->state = PARSER_STATE_PAYLOAD;
            }
            break;

        /* Waiting for Payload[] bytes */
        case( PARSER_STATE_PAYLOAD ):
            parser->payload[parser->payloadBytesReceived++] = byte;
            //parser->payloadSum = (unsigned char)(parser->payloadSum + byte);
            parser->payloadSum = static_cast<unsigned char>(parser->payloadSum + byte);
            if( parser->payloadBytesReceived >= parser->payloadLength ) {
                parser->state = PARSER_STATE_CHKSUM;
            }
            break;

        /* Waiting for CKSUM byte */
        case( PARSER_STATE_CHKSUM ):
            parser->chksum = byte;
            parser->state = PARSER_STATE_SYNC;
            if( parser->chksum != ((~parser->payloadSum)&0xFF) ) {
                returnValue = -2;
            } else {
                returnValue = 1;
                parsePacketPayload( parser );
            }
            break;

        /* Waiting for high byte of 2-byte raw value */
        case( PARSER_STATE_WAIT_HIGH ):

            /* Check if current byte is a high byte */
            if( (byte & 0xC0) == 0x80 ) {
                /* High byte recognized, will be saved as parser->lastByte */
                parser->state = PARSER_STATE_WAIT_LOW;
            }
            break;

        /* Waiting for low byte of 2-byte raw value */
        case( PARSER_STATE_WAIT_LOW ):

            /* Check if current byte is a valid low byte */
            if( (byte & 0xC0) == 0x40 ) {

                /* Stuff the high and low part of the raw value into an array */
                parser->payload[0] = parser->lastByte;
                parser->payload[1] = byte;

                /* Notify the handler function of received raw value */
                if( parser->handleDataValue ) {
                    parser->handleDataValue( 0, PARSER_CODE_RAW_SIGNAL, 2,
                                             parser->payload,
                                             parser->customData );
                }

                returnValue = 1;
            }

            /* Return to start state waiting for high */
            parser->state = PARSER_STATE_WAIT_HIGH;

            break;

        /* 未识别时状态 */
        default:
            parser->state = PARSER_STATE_SYNC;
            returnValue = -5;
            break;
    }

    /* 保存现在的位*/
    parser->lastByte = byte;

    return( returnValue );
}


/**
 * Parses each row of data from the @c packet's Data[] block,
 * updating the fields of @c data as appropriate.
 */
static int
parsePacketPayload( ThinkGearStreamParser *parser ) {

    unsigned char i = 0;
    unsigned char extendedCodeLevel = 0;
    unsigned char code = 0;
    unsigned char numBytes = 0;

    /* 从payload[]解析所有源数据位 */
    while( i < parser->payloadLength ) {

        /* Parse possible EXtended CODE bytes */
        while( parser->payload[i] == PARSER_EXCODE_BYTE ) {
            extendedCodeLevel++;
            i++;
        }

        /* Parse CODE */
        code = parser->payload[i++];

        /* Parse value length */
        if( code >= 0x80 ) numBytes = parser->payload[i++];
        else               numBytes = 1;

        /* Call the callback function to handle the DataRow value */
        if( parser->handleDataValue ) {
            parser->handleDataValue( extendedCodeLevel, code, numBytes,
                                     parser->payload+i, parser->customData );
        }
        //i = (unsigned char)(i + numBytes);

        i= static_cast<unsigned char>(i+numBytes);
    }
    return( 0 );
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif // THINKGEARSTREAMPARSER_HPP
