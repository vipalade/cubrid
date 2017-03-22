/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_CSQL_YY_HOME_VALENTINP_WORK_CUBRID_SRC_PARSER_CSQL_GRAMMAR_H_INCLUDED
# define YY_CSQL_YY_HOME_VALENTINP_WORK_CUBRID_SRC_PARSER_CSQL_GRAMMAR_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int csql_yydebug;
#endif
/* "%code requires" blocks.  */


#include "parser.h"

/* 
 * The default YYLTYPE structure is extended so that locations can hold
 * context information
 */
typedef struct YYLTYPE
{

  int first_line;
  int first_column;
  int last_line;
  int last_column;
  int buffer_pos; /* position in the buffer being parsed */

} YYLTYPE;
#define YYLTYPE_IS_DECLARED 1

typedef struct
{
  PT_NODE *c1;
  PT_NODE *c2;
} container_2;

typedef struct
{
  PT_NODE *c1;
  PT_NODE *c2;
  PT_NODE *c3;
} container_3;

typedef struct
{
  PT_NODE *c1;
  PT_NODE *c2;
  PT_NODE *c3;
  PT_NODE *c4;
} container_4;

typedef struct
{
  PT_NODE *c1;
  PT_NODE *c2;
  PT_NODE *c3;
  PT_NODE *c4;
  PT_NODE *c5;
  PT_NODE *c6;
  PT_NODE *c7;
  PT_NODE *c8;
  PT_NODE *c9;
  PT_NODE *c10;
} container_10;

void csql_yyerror_explicit (int line, int column);
void csql_yyerror (const char *s);

extern int g_msg[1024];
extern int msg_ptr;
extern int yybuffer_pos;



/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ABSOLUTE_ = 258,
    ACTION = 259,
    ADD = 260,
    ADD_MONTHS = 261,
    AFTER = 262,
    ALL = 263,
    ALLOCATE = 264,
    ALTER = 265,
    AND = 266,
    ANY = 267,
    ARE = 268,
    AS = 269,
    ASC = 270,
    ASSERTION = 271,
    ASYNC = 272,
    AT = 273,
    ATTACH = 274,
    ATTRIBUTE = 275,
    AVG = 276,
    BEFORE = 277,
    BEGIN_ = 278,
    BETWEEN = 279,
    BIGINT = 280,
    BINARY = 281,
    BIT = 282,
    BIT_LENGTH = 283,
    BITSHIFT_LEFT = 284,
    BITSHIFT_RIGHT = 285,
    BLOB_ = 286,
    BOOLEAN_ = 287,
    BOTH_ = 288,
    BREADTH = 289,
    BY = 290,
    CALL = 291,
    CASCADE = 292,
    CASCADED = 293,
    CASE = 294,
    CAST = 295,
    CATALOG = 296,
    CHANGE = 297,
    CHAR_ = 298,
    CHECK = 299,
    CLASS = 300,
    CLASSES = 301,
    CLOB_ = 302,
    CLOSE = 303,
    COALESCE = 304,
    COLLATE = 305,
    COLUMN = 306,
    COMMIT = 307,
    COMP_NULLSAFE_EQ = 308,
    CONNECT = 309,
    CONNECT_BY_ISCYCLE = 310,
    CONNECT_BY_ISLEAF = 311,
    CONNECT_BY_ROOT = 312,
    CONNECTION = 313,
    CONSTRAINT = 314,
    CONSTRAINTS = 315,
    CONTINUE = 316,
    CONVERT = 317,
    CORRESPONDING = 318,
    COUNT = 319,
    CREATE = 320,
    CROSS = 321,
    CURRENT = 322,
    CURRENT_DATE = 323,
    CURRENT_DATETIME = 324,
    CURRENT_TIME = 325,
    CURRENT_TIMESTAMP = 326,
    CURRENT_USER = 327,
    CURSOR = 328,
    CYCLE = 329,
    DATA = 330,
    DATABASE = 331,
    DATA_TYPE_ = 332,
    Date = 333,
    DATETIME = 334,
    DATETIMETZ = 335,
    DATETIMELTZ = 336,
    DAY_ = 337,
    DAY_MILLISECOND = 338,
    DAY_SECOND = 339,
    DAY_MINUTE = 340,
    DAY_HOUR = 341,
    DB_TIMEZONE = 342,
    DEALLOCATE = 343,
    DECLARE = 344,
    DEFAULT = 345,
    DEFERRABLE = 346,
    DEFERRED = 347,
    DELETE_ = 348,
    DEPTH = 349,
    DESC = 350,
    DESCRIBE = 351,
    DESCRIPTOR = 352,
    DIAGNOSTICS = 353,
    DIFFERENCE_ = 354,
    DISCONNECT = 355,
    DISTINCT = 356,
    DIV = 357,
    DO = 358,
    Domain = 359,
    Double = 360,
    DROP = 361,
    DUPLICATE_ = 362,
    EACH = 363,
    ELSE = 364,
    ELSEIF = 365,
    END = 366,
    ENUM = 367,
    EQUALS = 368,
    ESCAPE = 369,
    EVALUATE = 370,
    EXCEPT = 371,
    EXCEPTION = 372,
    EXEC = 373,
    EXECUTE = 374,
    EXISTS = 375,
    EXTERNAL = 376,
    EXTRACT = 377,
    False = 378,
    FETCH = 379,
    File = 380,
    FIRST = 381,
    FLOAT_ = 382,
    For = 383,
    FORCE = 384,
    FOREIGN = 385,
    FOUND = 386,
    FROM = 387,
    FULL = 388,
    FUNCTION = 389,
    GENERAL = 390,
    GET = 391,
    GLOBAL = 392,
    GO = 393,
    GOTO = 394,
    GRANT = 395,
    GROUP_ = 396,
    HAVING = 397,
    HOUR_ = 398,
    HOUR_MILLISECOND = 399,
    HOUR_SECOND = 400,
    HOUR_MINUTE = 401,
    IDENTITY = 402,
    IF = 403,
    IGNORE_ = 404,
    IMMEDIATE = 405,
    IN_ = 406,
    INDEX = 407,
    INDICATOR = 408,
    INHERIT = 409,
    INITIALLY = 410,
    INNER = 411,
    INOUT = 412,
    INPUT_ = 413,
    INSERT = 414,
    INTEGER = 415,
    INTERNAL = 416,
    INTERSECT = 417,
    INTERSECTION = 418,
    INTERVAL = 419,
    INTO = 420,
    IS = 421,
    ISOLATION = 422,
    JOIN = 423,
    KEY = 424,
    KEYLIMIT = 425,
    LANGUAGE = 426,
    LAST = 427,
    LEADING_ = 428,
    LEAVE = 429,
    LEFT = 430,
    LESS = 431,
    LEVEL = 432,
    LIKE = 433,
    LIMIT = 434,
    LIST = 435,
    LOCAL = 436,
    LOCAL_TRANSACTION_ID = 437,
    LOCALTIME = 438,
    LOCALTIMESTAMP = 439,
    LOOP = 440,
    LOWER = 441,
    MATCH = 442,
    MATCHED = 443,
    Max = 444,
    MERGE = 445,
    METHOD = 446,
    MILLISECOND_ = 447,
    Min = 448,
    MINUTE_ = 449,
    MINUTE_MILLISECOND = 450,
    MINUTE_SECOND = 451,
    MOD = 452,
    MODIFY = 453,
    MODULE = 454,
    Monetary = 455,
    MONTH_ = 456,
    MULTISET = 457,
    MULTISET_OF = 458,
    NA = 459,
    NAMES = 460,
    NATIONAL = 461,
    NATURAL = 462,
    NCHAR = 463,
    NEXT = 464,
    NO = 465,
    NOT = 466,
    Null = 467,
    NULLIF = 468,
    NUMERIC = 469,
    OBJECT = 470,
    OCTET_LENGTH = 471,
    OF = 472,
    OFF_ = 473,
    ON_ = 474,
    ONLY = 475,
    OPEN = 476,
    OPTIMIZATION = 477,
    OPTION = 478,
    OR = 479,
    ORDER = 480,
    OUT_ = 481,
    OUTER = 482,
    OUTPUT = 483,
    OVER = 484,
    OVERLAPS = 485,
    PARAMETERS = 486,
    PARTIAL = 487,
    PARTITION = 488,
    POSITION = 489,
    PRECISION = 490,
    PREPARE = 491,
    PRESERVE = 492,
    PRIMARY = 493,
    PRIOR = 494,
    PRIVILEGES = 495,
    PROCEDURE = 496,
    PROMOTE = 497,
    QUERY = 498,
    READ = 499,
    REBUILD = 500,
    RECURSIVE = 501,
    REF = 502,
    REFERENCES = 503,
    REFERENCING = 504,
    REGEXP = 505,
    RELATIVE_ = 506,
    RENAME = 507,
    REPLACE = 508,
    RESIGNAL = 509,
    RESTRICT = 510,
    RETURN = 511,
    RETURNS = 512,
    REVOKE = 513,
    RIGHT = 514,
    RLIKE = 515,
    ROLE = 516,
    ROLLBACK = 517,
    ROLLUP = 518,
    ROUTINE = 519,
    ROW = 520,
    ROWNUM = 521,
    ROWS = 522,
    SAVEPOINT = 523,
    SCHEMA = 524,
    SCOPE = 525,
    SCROLL = 526,
    SEARCH = 527,
    SECOND_ = 528,
    SECOND_MILLISECOND = 529,
    SECTION = 530,
    SELECT = 531,
    SENSITIVE = 532,
    SEQUENCE = 533,
    SEQUENCE_OF = 534,
    SERIALIZABLE = 535,
    SESSION = 536,
    SESSION_TIMEZONE = 537,
    SESSION_USER = 538,
    SET = 539,
    SET_OF = 540,
    SETEQ = 541,
    SETNEQ = 542,
    SHARED = 543,
    SIBLINGS = 544,
    SIGNAL = 545,
    SIMILAR = 546,
    SIZE_ = 547,
    SmallInt = 548,
    SOME = 549,
    SQL = 550,
    SQLCODE = 551,
    SQLERROR = 552,
    SQLEXCEPTION = 553,
    SQLSTATE = 554,
    SQLWARNING = 555,
    STATISTICS = 556,
    String = 557,
    SUBCLASS = 558,
    SUBSET = 559,
    SUBSETEQ = 560,
    SUBSTRING_ = 561,
    SUM = 562,
    SUPERCLASS = 563,
    SUPERSET = 564,
    SUPERSETEQ = 565,
    SYS_CONNECT_BY_PATH = 566,
    SYS_DATE = 567,
    SYS_DATETIME = 568,
    SYS_TIME_ = 569,
    SYS_TIMESTAMP = 570,
    SYSTEM_USER = 571,
    TABLE = 572,
    TEMPORARY = 573,
    THEN = 574,
    Time = 575,
    TIMESTAMP = 576,
    TIMESTAMPTZ = 577,
    TIMESTAMPLTZ = 578,
    TIMEZONE = 579,
    TIMEZONE_HOUR = 580,
    TIMEZONE_MINUTE = 581,
    TO = 582,
    TRAILING_ = 583,
    TRANSACTION = 584,
    TRANSLATE = 585,
    TRANSLATION = 586,
    TRIGGER = 587,
    TRIM = 588,
    True = 589,
    TRUNCATE = 590,
    UNDER = 591,
    Union = 592,
    UNIQUE = 593,
    UNKNOWN = 594,
    UNTERMINATED_STRING = 595,
    UNTERMINATED_IDENTIFIER = 596,
    UPDATE = 597,
    UPPER = 598,
    USAGE = 599,
    USE = 600,
    USER = 601,
    USING = 602,
    Utime = 603,
    VACUUM = 604,
    VALUE = 605,
    VALUES = 606,
    VAR_ASSIGN = 607,
    VARCHAR = 608,
    VARIABLE_ = 609,
    VARYING = 610,
    VCLASS = 611,
    VIEW = 612,
    WHEN = 613,
    WHENEVER = 614,
    WHERE = 615,
    WHILE = 616,
    WITH = 617,
    WITHOUT = 618,
    WORK = 619,
    WRITE = 620,
    XOR = 621,
    YEAR_ = 622,
    YEAR_MONTH = 623,
    ZONE = 624,
    YEN_SIGN = 625,
    DOLLAR_SIGN = 626,
    WON_SIGN = 627,
    TURKISH_LIRA_SIGN = 628,
    BRITISH_POUND_SIGN = 629,
    CAMBODIAN_RIEL_SIGN = 630,
    CHINESE_RENMINBI_SIGN = 631,
    INDIAN_RUPEE_SIGN = 632,
    RUSSIAN_RUBLE_SIGN = 633,
    AUSTRALIAN_DOLLAR_SIGN = 634,
    CANADIAN_DOLLAR_SIGN = 635,
    BRASILIAN_REAL_SIGN = 636,
    ROMANIAN_LEU_SIGN = 637,
    EURO_SIGN = 638,
    SWISS_FRANC_SIGN = 639,
    DANISH_KRONE_SIGN = 640,
    NORWEGIAN_KRONE_SIGN = 641,
    BULGARIAN_LEV_SIGN = 642,
    VIETNAMESE_DONG_SIGN = 643,
    CZECH_KORUNA_SIGN = 644,
    POLISH_ZLOTY_SIGN = 645,
    SWEDISH_KRONA_SIGN = 646,
    CROATIAN_KUNA_SIGN = 647,
    SERBIAN_DINAR_SIGN = 648,
    DOT = 649,
    RIGHT_ARROW = 650,
    STRCAT = 651,
    COMP_NOT_EQ = 652,
    COMP_GE = 653,
    COMP_LE = 654,
    PARAM_HEADER = 655,
    ACCESS = 656,
    ACTIVE = 657,
    ADDDATE = 658,
    ANALYZE = 659,
    ARCHIVE = 660,
    AUTO_INCREMENT = 661,
    BIT_AND = 662,
    BIT_OR = 663,
    BIT_XOR = 664,
    CACHE = 665,
    CAPACITY = 666,
    CHARACTER_SET_ = 667,
    CHARSET = 668,
    CHR = 669,
    CLOB_TO_CHAR = 670,
    COLLATION = 671,
    COLUMNS = 672,
    COMMENT = 673,
    COMMITTED = 674,
    COST = 675,
    CRITICAL = 676,
    CUME_DIST = 677,
    DATE_ADD = 678,
    DATE_SUB = 679,
    DECREMENT = 680,
    DENSE_RANK = 681,
    ELT = 682,
    COMBINE = 683,
    EXPLAIN = 684,
    FIRST_VALUE = 685,
    FULLSCAN = 686,
    GE_INF_ = 687,
    GE_LE_ = 688,
    GE_LT_ = 689,
    GRANTS = 690,
    GROUP_CONCAT = 691,
    GROUPS = 692,
    GT_INF_ = 693,
    GT_LE_ = 694,
    GT_LT_ = 695,
    HASH = 696,
    HEADER = 697,
    HEAP = 698,
    IFNULL = 699,
    INACTIVE = 700,
    INCREMENT = 701,
    INDEXES = 702,
    INDEX_PREFIX = 703,
    INF_LE_ = 704,
    INF_LT_ = 705,
    INFINITE_ = 706,
    INSTANCES = 707,
    INVALIDATE = 708,
    ISNULL = 709,
    KEYS = 710,
    KILL = 711,
    JAVA = 712,
    JOB = 713,
    JSON = 714,
    LAG = 715,
    LAST_VALUE = 716,
    LCASE = 717,
    LEAD = 718,
    LOCK_ = 719,
    LOG = 720,
    MAXIMUM = 721,
    MAXVALUE = 722,
    MEDIAN = 723,
    MEMBERS = 724,
    MINVALUE = 725,
    NAME = 726,
    NOCYCLE = 727,
    NOCACHE = 728,
    NOMAXVALUE = 729,
    NOMINVALUE = 730,
    NONE = 731,
    NTH_VALUE = 732,
    NTILE = 733,
    NULLS = 734,
    OFFSET = 735,
    OWNER = 736,
    PAGE = 737,
    PARTITIONING = 738,
    PARTITIONS = 739,
    PASSWORD = 740,
    PERCENT_RANK = 741,
    PERCENTILE_CONT = 742,
    PERCENTILE_DISC = 743,
    PRINT = 744,
    PRIORITY = 745,
    QUARTER = 746,
    QUEUES = 747,
    RANGE_ = 748,
    RANK = 749,
    REJECT_ = 750,
    REMOVE = 751,
    REORGANIZE = 752,
    REPEATABLE = 753,
    RESPECT = 754,
    RETAIN = 755,
    REUSE_OID = 756,
    REVERSE = 757,
    DISK_SIZE = 758,
    ROW_NUMBER = 759,
    SECTIONS = 760,
    SEPARATOR = 761,
    SERIAL = 762,
    SHOW = 763,
    SLEEP = 764,
    SLOTS = 765,
    SLOTTED = 766,
    STABILITY = 767,
    START_ = 768,
    STATEMENT = 769,
    STATUS = 770,
    STDDEV = 771,
    STDDEV_POP = 772,
    STDDEV_SAMP = 773,
    STR_TO_DATE = 774,
    SUBDATE = 775,
    SYSTEM = 776,
    TABLES = 777,
    TEXT = 778,
    THAN = 779,
    THREADS = 780,
    TIMEOUT = 781,
    TRACE = 782,
    TRAN = 783,
    TRIGGERS = 784,
    UCASE = 785,
    UNCOMMITTED = 786,
    VAR_POP = 787,
    VAR_SAMP = 788,
    VARIANCE = 789,
    VOLUME = 790,
    WEEK = 791,
    WITHIN = 792,
    WORKSPACE = 793,
    TIMEZONES = 794,
    IdName = 795,
    BracketDelimitedIdName = 796,
    BacktickDelimitedIdName = 797,
    DelimitedIdName = 798,
    UNSIGNED_INTEGER = 799,
    UNSIGNED_REAL = 800,
    CHAR_STRING = 801,
    NCHAR_STRING = 802,
    BIT_STRING = 803,
    HEX_STRING = 804,
    CPP_STYLE_HINT = 805,
    C_STYLE_HINT = 806,
    SQL_STYLE_HINT = 807,
    BINARY_STRING = 808,
    EUCKR_STRING = 809,
    ISO_STRING = 810,
    UTF8_STRING = 811
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{


  int number;
  bool boolean;
  PT_NODE *node;
  char *cptr;
  container_2 c2;
  container_3 c3;
  container_4 c4;
  container_10 c10;


};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE csql_yylval;
extern YYLTYPE csql_yylloc;
int csql_yyparse (void);

#endif /* !YY_CSQL_YY_HOME_VALENTINP_WORK_CUBRID_SRC_PARSER_CSQL_GRAMMAR_H_INCLUDED  */
