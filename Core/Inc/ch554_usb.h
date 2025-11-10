/*--------------------------------------------------------------------------
CH554.H
Header file for CH554 microcontrollers.
****************************************
**  Copyright  (C)  W.ch  1999-2014   **
**  Web:              http://wch.cn   **
****************************************
--------------------------------------------------------------------------*/


#ifndef __USB_DEF__
#define __USB_DEF__

#include "ch554.h"
/*----- USB constant and structure define --------------------------------*/

/* USB PID */
#ifndef USB_PID_SETUP
#define USB_PID_NULL            0x00    /* reserved PID */
#define USB_PID_SOF             0x05
#define USB_PID_SETUP           0x0D
#define USB_PID_IN              0x09
#define USB_PID_OUT             0x01
#define USB_PID_ACK             0x02
#define USB_PID_NAK             0x0A
#define USB_PID_STALL           0x0E
#define USB_PID_DATA0           0x03
#define USB_PID_DATA1           0x0B
#define USB_PID_PRE             0x0C
#endif

/* USB standard device request code */
#ifndef USB_GET_DESCRIPTOR
#define USB_GET_STATUS          0x00
#define USB_CLEAR_FEATURE       0x01
#define USB_SET_FEATURE         0x03
#define USB_SET_ADDRESS         0x05
#define USB_GET_DESCRIPTOR      0x06
#define USB_SET_DESCRIPTOR      0x07
#define USB_GET_CONFIGURATION   0x08
#define USB_SET_CONFIGURATION   0x09
#define USB_GET_INTERFACE       0x0A
#define USB_SET_INTERFACE       0x0B
#define USB_SYNCH_FRAME         0x0C
#endif

/* USB hub class request code */
#ifndef HUB_GET_DESCRIPTOR
#define HUB_GET_STATUS          0x00
#define HUB_CLEAR_FEATURE       0x01
#define HUB_GET_STATE           0x02
#define HUB_SET_FEATURE         0x03
#define HUB_GET_DESCRIPTOR      0x06
#define HUB_SET_DESCRIPTOR      0x07
#endif

/* USB HID class request code */
#ifndef HID_GET_REPORT
#define HID_GET_REPORT          0x01
#define HID_GET_IDLE            0x02
#define HID_GET_PROTOCOL        0x03
#define HID_SET_REPORT          0x09
#define HID_SET_IDLE            0x0A
#define HID_SET_PROTOCOL        0x0B
#endif

/* Bit define for USB request type */
#ifndef USB_REQ_TYP_MASK
#define USB_REQ_TYP_IN          0x80            /* control IN, device to host */
#define USB_REQ_TYP_OUT         0x00            /* control OUT, host to device */
#define USB_REQ_TYP_READ        0x80            /* control read, device to host */
#define USB_REQ_TYP_WRITE       0x00            /* control write, host to device */
#define USB_REQ_TYP_MASK        0x60            /* bit mask of request type */
#define USB_REQ_TYP_STANDARD    0x00
#define USB_REQ_TYP_CLASS       0x20
#define USB_REQ_TYP_VENDOR      0x40
#define USB_REQ_TYP_RESERVED    0x60
#define USB_REQ_RECIP_MASK      0x1F            /* bit mask of request recipient */
#define USB_REQ_RECIP_DEVICE    0x00
#define USB_REQ_RECIP_INTERF    0x01
#define USB_REQ_RECIP_ENDP      0x02
#define USB_REQ_RECIP_OTHER     0x03
#endif

/* USB request type for hub class request */
#ifndef HUB_GET_HUB_DESCRIPTOR
#define HUB_CLEAR_HUB_FEATURE   0x20
#define HUB_CLEAR_PORT_FEATURE  0x23
#define HUB_GET_BUS_STATE       0xA3
#define HUB_GET_HUB_DESCRIPTOR  0xA0
#define HUB_GET_HUB_STATUS      0xA0
#define HUB_GET_PORT_STATUS     0xA3
#define HUB_SET_HUB_DESCRIPTOR  0x20
#define HUB_SET_HUB_FEATURE     0x20
#define HUB_SET_PORT_FEATURE    0x23
#endif

/* Hub class feature selectors */
#ifndef HUB_PORT_RESET
#define HUB_C_HUB_LOCAL_POWER   0
#define HUB_C_HUB_OVER_CURRENT  1
#define HUB_PORT_CONNECTION     0
#define HUB_PORT_ENABLE         1
#define HUB_PORT_SUSPEND        2
#define HUB_PORT_OVER_CURRENT   3
#define HUB_PORT_RESET          4
#define HUB_PORT_POWER          8
#define HUB_PORT_LOW_SPEED      9
#define HUB_C_PORT_CONNECTION   16
#define HUB_C_PORT_ENABLE       17
#define HUB_C_PORT_SUSPEND      18
#define HUB_C_PORT_OVER_CURRENT 19
#define HUB_C_PORT_RESET        20
#endif

/* USB descriptor type */
#ifndef USB_DESCR_TYP_DEVICE
#define USB_DESCR_TYP_DEVICE    0x01
#define USB_DESCR_TYP_CONFIG    0x02
#define USB_DESCR_TYP_STRING    0x03
#define USB_DESCR_TYP_INTERF    0x04
#define USB_DESCR_TYP_ENDP      0x05
#define USB_DESCR_TYP_QUALIF    0x06
#define USB_DESCR_TYP_SPEED     0x07
#define USB_DESCR_TYP_OTG       0x09
#define USB_DESCR_TYP_HID       0x21
#define USB_DESCR_TYP_REPORT    0x22
#define USB_DESCR_TYP_PHYSIC    0x23
#define USB_DESCR_TYP_CS_INTF   0x24
#define USB_DESCR_TYP_CS_ENDP   0x25
#define USB_DESCR_TYP_HUB       0x29
#endif

/* USB device class */
#ifndef USB_DEV_CLASS_HUB
#define USB_DEV_CLASS_RESERVED  0x00
#define USB_DEV_CLASS_AUDIO     0x01
#define USB_DEV_CLASS_COMMUNIC  0x02
#define USB_DEV_CLASS_HID       0x03
#define USB_DEV_CLASS_MONITOR   0x04
#define USB_DEV_CLASS_PHYSIC_IF 0x05
#define USB_DEV_CLASS_POWER     0x06
#define USB_DEV_CLASS_PRINTER   0x07
#define USB_DEV_CLASS_STORAGE   0x08
#define USB_DEV_CLASS_HUB       0x09
#define USB_DEV_CLASS_VEN_SPEC  0xFF
#endif

/* USB endpoint type and attributes */
#ifndef USB_ENDP_TYPE_MASK
#define USB_ENDP_DIR_MASK       0x80
#define USB_ENDP_ADDR_MASK      0x0F
#define USB_ENDP_TYPE_MASK      0x03
#define USB_ENDP_TYPE_CTRL      0x00
#define USB_ENDP_TYPE_ISOCH     0x01
#define USB_ENDP_TYPE_BULK      0x02
#define USB_ENDP_TYPE_INTER     0x03
#endif

#ifndef USB_DEVICE_ADDR
#define	USB_DEVICE_ADDR			0x02	/* Ĭ�ϵ�USB�豸��ַ */
#endif
#ifndef DEFAULT_ENDP0_SIZE
#define DEFAULT_ENDP0_SIZE      8       /* default maximum packet size for endpoint 0 */
#endif
#ifndef DEFAULT_ENDP1_SIZE
#define DEFAULT_ENDP1_SIZE      8       /* default maximum packet size for endpoint 1 */
#endif
#ifndef MAX_PACKET_SIZE
#define MAX_PACKET_SIZE         64      /* maximum packet size */
#endif
#ifndef USB_BO_CBW_SIZE
#define USB_BO_CBW_SIZE			0x1F	/* �����CBW���ܳ��� */
#define USB_BO_CSW_SIZE			0x0D	/* ����״̬��CSW���ܳ��� */
#endif
#ifndef USB_BO_CBW_SIG0
#define USB_BO_CBW_SIG0         0x55    /* �����CBWʶ���־'USBC' */
#define USB_BO_CBW_SIG1         0x53
#define USB_BO_CBW_SIG2         0x42
#define USB_BO_CBW_SIG3         0x43
#define USB_BO_CSW_SIG0         0x55    /* ����״̬��CSWʶ���־'USBS' */
#define USB_BO_CSW_SIG1         0x53
#define USB_BO_CSW_SIG2         0x42
#define USB_BO_CSW_SIG3         0x53
#endif

#define CPU_TO_LE16(x)           (x)

#define VERSION_BCD(Major, Minor, Revision) \
										  CPU_TO_LE16( ((Major & 0xFF) << 8) | \
													   ((Minor & 0x0F) << 4) | \
													   (Revision & 0x0F) )

#define FIXED_CONTROL_ENDPOINT_SIZE      8
#define FIXED_NUM_CONFIGURATIONS         1

#define NO_DESCRIPTOR                    0
#define USE_INTERNAL_SERIAL 			 NO_DESCRIPTOR
#define USB_CONFIG_ATTR_RESERVED          0x80
#define USB_CONFIG_ATTR_REMOTEWAKEUP      0x20

/** Endpoint address direction mask for an IN direction (Device to Host) endpoint. This may be ORed with
 *  the index of the address within a device to obtain the full endpoint address.
 */
#define ENDPOINT_DIR_IN                    0x80
#define MOUSE_EPADDR                   (ENDPOINT_DIR_IN  | 1)

#define EP_TYPE_INTERRUPT                  0x03
#define ENDPOINT_ATTR_NO_SYNC             (0 << 2)
#define ENDPOINT_USAGE_DATA               (0 << 4)

/** Size in bytes of the Mouse HID reporting IN endpoint. */
#define MOUSE_EPSIZE                   8
#define USB_CONFIG_POWER_MA(mA)           ((mA) >> 1)

/** Enum for possible Class, Subclass and Protocol values of device and interface descriptors. */
enum USB_Descriptor_ClassSubclassProtocol_t
{
	USB_CSCP_NoDeviceClass          = 0x00, /**< Descriptor Class value indicating that the device does not belong
											 *   to a particular class at the device level.
											 */
	USB_CSCP_NoDeviceSubclass       = 0x00, /**< Descriptor Subclass value indicating that the device does not belong
											 *   to a particular subclass at the device level.
											 */
	USB_CSCP_NoDeviceProtocol       = 0x00, /**< Descriptor Protocol value indicating that the device does not belong
											 *   to a particular protocol at the device level.
											 */
	USB_CSCP_VendorSpecificClass    = 0xFF, /**< Descriptor Class value indicating that the device/interface belongs
											 *   to a vendor specific class.
											 */
	USB_CSCP_VendorSpecificSubclass = 0xFF, /**< Descriptor Subclass value indicating that the device/interface belongs
											 *   to a vendor specific subclass.
											 */
	USB_CSCP_VendorSpecificProtocol = 0xFF, /**< Descriptor Protocol value indicating that the device/interface belongs
											 *   to a vendor specific protocol.
											 */
	USB_CSCP_IADDeviceClass         = 0xEF, /**< Descriptor Class value indicating that the device belongs to the
											 *   Interface Association Descriptor class.
											 */
	USB_CSCP_IADDeviceSubclass      = 0x02, /**< Descriptor Subclass value indicating that the device belongs to the
											 *   Interface Association Descriptor subclass.
											 */
	USB_CSCP_IADDeviceProtocol      = 0x01, /**< Descriptor Protocol value indicating that the device belongs to the
											 *   Interface Association Descriptor protocol.
											 */
};

/* Type Defines: */
	/** Enum for possible Class, Subclass and Protocol values of device and interface descriptors relating to the HID
	 *  device class.
	 */
	enum HID_Descriptor_ClassSubclassProtocol_t
	{
		HID_CSCP_HIDClass             = 0x03, /**< Descriptor Class value indicating that the device or interface
		                                       *   belongs to the HID class.
		                                       */
		HID_CSCP_NonBootSubclass      = 0x00, /**< Descriptor Subclass value indicating that the device or interface
		                                       *   does not implement a HID boot protocol.
		                                       */
		HID_CSCP_BootSubclass         = 0x01, /**< Descriptor Subclass value indicating that the device or interface
		                                       *   implements a HID boot protocol.
		                                       */
		HID_CSCP_NonBootProtocol      = 0x00, /**< Descriptor Protocol value indicating that the device or interface
		                                       *   does not belong to a HID boot protocol.
		                                       */
		HID_CSCP_KeyboardBootProtocol = 0x01, /**< Descriptor Protocol value indicating that the device or interface
		                                       *   belongs to the Keyboard HID boot protocol.
		                                       */
		HID_CSCP_MouseBootProtocol    = 0x02, /**< Descriptor Protocol value indicating that the device or interface
		                                       *   belongs to the Mouse HID boot protocol.
		                                       */
	};

/** Enum for the HID class specific descriptor types. */
enum HID_DescriptorTypes_t
{
	HID_DTYPE_HID           = 0x21, /**< Descriptor header type value, to indicate a HID class HID descriptor. */
	HID_DTYPE_Report        = 0x22, /**< Descriptor header type value, to indicate a HID class HID report descriptor. */
};

/* Enum for the possible standard descriptor types, as given in each descriptor's header. */
enum USB_DescriptorTypes_t
{
	DTYPE_Device                    = 0x01, /**< Indicates that the descriptor is a device descriptor. */
	DTYPE_Configuration             = 0x02, /**< Indicates that the descriptor is a configuration descriptor. */
	DTYPE_String                    = 0x03, /**< Indicates that the descriptor is a string descriptor. */
	DTYPE_Interface                 = 0x04, /**< Indicates that the descriptor is an interface descriptor. */
	DTYPE_Endpoint                  = 0x05, /**< Indicates that the descriptor is an endpoint descriptor. */
	DTYPE_DeviceQualifier           = 0x06, /**< Indicates that the descriptor is a device qualifier descriptor. */
	DTYPE_Other                     = 0x07, /**< Indicates that the descriptor is of other type. */
	DTYPE_InterfacePower            = 0x08, /**< Indicates that the descriptor is an interface power descriptor. */
	DTYPE_InterfaceAssociation      = 0x0B, /**< Indicates that the descriptor is an interface association descriptor. */
	DTYPE_CSInterface               = 0x24, /**< Indicates that the descriptor is a class specific interface descriptor. */
	DTYPE_CSEndpoint                = 0x25, /**< Indicates that the descriptor is a class specific endpoint descriptor. */
};

typedef struct _USB_SETUP_REQ {
    uint8_t bRequestType;
    uint8_t bRequest;
    uint8_t wValueL;
    uint8_t wValueH;
    uint8_t wIndexL;
    uint8_t wIndexH;
    uint8_t wLengthL;
    uint8_t wLengthH;
} USB_SETUP_REQ, *PUSB_SETUP_REQ;

typedef USB_SETUP_REQ __xdata *PXUSB_SETUP_REQ;

//typedef struct _USB_DEVICE_DESCR {
//    uint8_t bLength;              /*< Size of the descriptor, in bytes. */
//    uint8_t bDescriptorType;      /*< Type of the descriptor, either a value in USB_DescriptorTypes_t or a value given by the specific class. */
//    uint8_t bcdUSBL;
//    uint8_t bcdUSBH;
//    uint8_t bDeviceClass;
//    uint8_t bDeviceSubClass;
//    uint8_t bDeviceProtocol;
//    uint8_t bMaxPacketSize0;
//    uint8_t idVendorL;
//    uint8_t idVendorH;
//    uint8_t idProductL;
//    uint8_t idProductH;
//    uint8_t bcdDeviceL;
//    uint8_t bcdDeviceH;
//    uint8_t iManufacturer;
//    uint8_t iProduct;
//    uint8_t iSerialNumber;
//    uint8_t bNumConfigurations;
//} USB_DEV_DESCR, *PUSB_DEV_DESCR;
//
//typedef USB_DEV_DESCR __xdata *PXUSB_DEV_DESCR;
//
/** Type define for the data type used to store HID report descriptor elements. */
typedef uint8_t USB_Descriptor_HIDReport_Datatype_t;

typedef struct
{
	uint8_t Size; /**< Size of the descriptor, in bytes. */
	uint8_t Type; /**< Type of the descriptor, either a value in \ref USB_DescriptorTypes_t or a value
	               *   given by the specific class.
	               */
} USB_Descriptor_Header_t;


typedef struct
{
	USB_Descriptor_Header_t Header; /**< Descriptor header, including type and size. */

	uint16_t TotalConfigurationSize; /**< Size of the configuration descriptor header,
	                                  *   and all sub descriptors inside the configuration.
	                                  */
	uint8_t  TotalInterfaces; /**< Total number of interfaces in the configuration. */

	uint8_t  ConfigurationNumber; /**< Configuration index of the current configuration. */
	uint8_t  ConfigurationStrIndex; /**< Index of a string descriptor describing the configuration. */

	uint8_t  ConfigAttributes; /**< Configuration attributes, comprised of a mask of \c USB_CONFIG_ATTR_* masks.
	                            *   On all devices, this should include USB_CONFIG_ATTR_RESERVED at a minimum.
	                            */

	uint8_t  MaxPowerConsumption; /**< Maximum power consumption of the device while in the
	                               *   current configuration, calculated by the \ref USB_CONFIG_POWER_MA()
	                               *   macro.
	                               */
} USB_Descriptor_Configuration_Header_t;

typedef struct
{
	USB_Descriptor_Header_t Header; /**< Descriptor header, including type and size. */

	uint8_t InterfaceNumber; /**< Index of the interface in the current configuration. */
	uint8_t AlternateSetting; /**< Alternate setting for the interface number. The same
	                           *   interface number can have multiple alternate settings
	                           *   with different endpoint configurations, which can be
	                           *   selected by the host.
	                           */
	uint8_t TotalEndpoints; /**< Total number of endpoints in the interface. */

	uint8_t Class; /**< Interface class ID. */
	uint8_t SubClass; /**< Interface subclass ID. */
	uint8_t Protocol; /**< Interface protocol ID. */

	uint8_t InterfaceStrIndex; /**< Index of the string descriptor describing the interface. */
} USB_Descriptor_Interface_t;


/** \brief HID class-specific HID Descriptor (LUFA naming conventions).
 *
 *  Type define for the HID class-specific HID descriptor, to describe the HID device's specifications. Refer to the HID
 *  specification for details on the structure elements.
 *
 *  \see \ref USB_HID_StdDescriptor_HID_t for the version of this type with standard element names.
 *
 *  \note Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{
	USB_Descriptor_Header_t Header; /**< Regular descriptor header containing the descriptor's type and length. */

	uint16_t                HIDSpec; /**< BCD encoded version that the HID descriptor and device complies to.
	                                  *
	                                  *   \see \ref VERSION_BCD() utility macro.
	                                  */
	uint8_t                 CountryCode; /**< Country code of the localized device, or zero if universal. */

	uint8_t                 TotalReportDescriptors; /**< Total number of HID report descriptors for the interface. */

	uint8_t                 HIDReportType; /**< Type of HID report, set to \ref HID_DTYPE_Report. */
	uint16_t                HIDReportLength; /**< Length of the associated HID report descriptor, in bytes. */
} USB_HID_Descriptor_HID_t;


typedef struct
{
	USB_Descriptor_Header_t Header; /**< Descriptor header, including type and size. */

	uint8_t  EndpointAddress; /**< Logical address of the endpoint within the device for the current
	                           *   configuration, including direction mask.
	                           */
	uint8_t  Attributes; /**< Endpoint attributes, comprised of a mask of the endpoint type (EP_TYPE_*)
	                      *   and attributes (ENDPOINT_ATTR_*) masks.
	                      */
	uint16_t EndpointSize; /**< Size of the endpoint bank, in bytes. This indicates the maximum packet
	                        *   size that the endpoint can receive at a time.
	                        */
	uint8_t  PollingIntervalMS; /**< Polling interval in milliseconds for the endpoint if it is an INTERRUPT
	                             *   or ISOCHRONOUS type.
	                             */
} USB_Descriptor_Endpoint_t;


typedef struct
{
	USB_Descriptor_Header_t Header; /**< Descriptor header, including type and size. */

	uint16_t USBSpecification; /**< BCD of the supported USB specification.
	                            *
	                            *   \see \ref VERSION_BCD() utility macro.
	                            */
	uint8_t  Class; /**< USB device class. */
	uint8_t  SubClass; /**< USB device subclass. */
	uint8_t  Protocol; /**< USB device protocol. */

	uint8_t  Endpoint0Size; /**< Size of the control (address 0) endpoint's bank in bytes. */

	uint16_t VendorID; /**< Vendor ID for the USB product. */
	uint16_t ProductID; /**< Unique product ID for the USB product. */
	uint16_t ReleaseNumber; /**< Product release (version) number.
	                         *
	                         *   \see \ref VERSION_BCD() utility macro.
	                         */
	uint8_t  ManufacturerStrIndex; /**< String index for the manufacturer's name. The
	                                *   host will request this string via a separate
	                                *   control request for the string descriptor.
	                                *
	                                *   \note If no string supplied, use \ref NO_DESCRIPTOR.
	                                */
	uint8_t  ProductStrIndex; /**< String index for the product name/details.
	                           *
	                           *  \see ManufacturerStrIndex structure entry.
	                           */
	uint8_t  SerialNumStrIndex; /**< String index for the product's globally unique hexadecimal
	                             *   serial number, in uppercase Unicode ASCII.
	                             *
	                             *  \note On some microcontroller models, there is an embedded serial number
	                             *        in the chip which can be used for the device serial number.
	                             *        To use this serial number, set this to \c USE_INTERNAL_SERIAL.
	                             *        On unsupported devices, this will evaluate to \ref NO_DESCRIPTOR
	                             *        and will cause the host to generate a pseudo-unique value for the
	                             *        device upon insertion.
	                             *
	                             *  \see \c ManufacturerStrIndex structure entry.
	                             */
	uint8_t  NumberOfConfigurations; /**< Total number of configurations supported by
	                                  *   the device.
	                                  */
} USB_Descriptor_Device_t;


/* Type Defines: */
	/** Type define for the device configuration descriptor structure. This must be defined in the
	 *  application code, as the configuration descriptor contains several sub-descriptors which
	 *  vary between devices, and which describe the device's usage to the host.
	 */
typedef struct
{
	USB_Descriptor_Configuration_Header_t    Config;

	// Digitizer HID Interface
	USB_Descriptor_Interface_t               HID_Interface;
	USB_HID_Descriptor_HID_t                 HID_DigitizerHID;
	USB_Descriptor_Endpoint_t                HID_ReportINEndpoint;
} USB_Descriptor_Configuration_t;








typedef struct _USB_CONFIG_DESCR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t wTotalLengthL;
    uint8_t wTotalLengthH;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t MaxPower;
} USB_CFG_DESCR, *PUSB_CFG_DESCR;

typedef USB_CFG_DESCR __xdata *PXUSB_CFG_DESCR;

typedef struct _USB_INTERF_DESCR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} USB_ITF_DESCR, *PUSB_ITF_DESCR;

typedef USB_ITF_DESCR __xdata *PXUSB_ITF_DESCR;

typedef struct _USB_ENDPOINT_DESCR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint8_t wMaxPacketSizeL;
    uint8_t wMaxPacketSizeH;
    uint8_t bInterval;
} USB_ENDP_DESCR, *PUSB_ENDP_DESCR;

typedef USB_ENDP_DESCR __xdata *PXUSB_ENDP_DESCR;

typedef struct _USB_CONFIG_DESCR_LONG {
    USB_CFG_DESCR   cfg_descr;
    USB_ITF_DESCR   itf_descr;
    USB_ENDP_DESCR  endp_descr[1];
} USB_CFG_DESCR_LONG, *PUSB_CFG_DESCR_LONG;

typedef USB_CFG_DESCR_LONG __xdata *PXUSB_CFG_DESCR_LONG;

typedef struct _USB_HUB_DESCR {
    uint8_t bDescLength;
    uint8_t bDescriptorType;
    uint8_t bNbrPorts;
    uint8_t wHubCharacteristicsL;
    uint8_t wHubCharacteristicsH;
    uint8_t bPwrOn2PwrGood;
    uint8_t bHubContrCurrent;
    uint8_t DeviceRemovable;
    uint8_t PortPwrCtrlMask;
} USB_HUB_DESCR, *PUSB_HUB_DESCR;

typedef USB_HUB_DESCR __xdata *PXUSB_HUB_DESCR;

typedef struct _USB_HID_DESCR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bcdHIDL;
    uint8_t bcdHIDH;
    uint8_t bCountryCode;
    uint8_t bNumDescriptors;
    uint8_t bDescriptorTypeX;
    uint8_t wDescriptorLengthL;
    uint8_t wDescriptorLengthH;
} USB_HID_DESCR, *PUSB_HID_DESCR;

typedef USB_HID_DESCR __xdata *PXUSB_HID_DESCR;

typedef struct _UDISK_BOC_CBW {         /* command of BulkOnly USB-FlashDisk */
    uint8_t mCBW_Sig0;
    uint8_t mCBW_Sig1;
    uint8_t mCBW_Sig2;
    uint8_t mCBW_Sig3;
    uint8_t mCBW_Tag0;
    uint8_t mCBW_Tag1;
    uint8_t mCBW_Tag2;
    uint8_t mCBW_Tag3;
    uint8_t mCBW_DataLen0;
    uint8_t mCBW_DataLen1;
    uint8_t mCBW_DataLen2;
    uint8_t mCBW_DataLen3;                /* uppest byte of data length, always is 0 */
    uint8_t mCBW_Flag;                    /* transfer direction and etc. */
    uint8_t mCBW_LUN;
    uint8_t mCBW_CB_Len;                  /* length of command block */
    uint8_t mCBW_CB_Buf[16];              /* command block buffer */
} UDISK_BOC_CBW, *PUDISK_BOC_CBW;

typedef UDISK_BOC_CBW __xdata *PXUDISK_BOC_CBW;

typedef struct _UDISK_BOC_CSW {         /* status of BulkOnly USB-FlashDisk */
    uint8_t mCSW_Sig0;
    uint8_t mCSW_Sig1;
    uint8_t mCSW_Sig2;
    uint8_t mCSW_Sig3;
    uint8_t mCSW_Tag0;
    uint8_t mCSW_Tag1;
    uint8_t mCSW_Tag2;
    uint8_t mCSW_Tag3;
    uint8_t mCSW_Residue0;                /* return: remainder bytes */
    uint8_t mCSW_Residue1;
    uint8_t mCSW_Residue2;
    uint8_t mCSW_Residue3;                /* uppest byte of remainder length, always is 0 */
    uint8_t mCSW_Status;                  /* return: result status */
} UDISK_BOC_CSW, *PUDISK_BOC_CSW;

typedef UDISK_BOC_CSW __xdata *PXUDISK_BOC_CSW;


//typedef struct
//{
//	// Collection: TouchScreen
//	uint8_t  ContactCountMaximum;						// Usage 0x000D0055: Contact Count Maximum, Value = 0 to 8
//} USB_DigitizerFeature_Data_t;


//typedef struct
//{
//	uint8_t ReportId;
//	USB_DigitizerFeature_Data_t Data;
//
//} USB_FeatureReport_t;


typedef struct
{
	// Collection: TouchScreen Finger(0)
	uint8_t  Finger0_TipSwitch : 1;                     // Usage 0x000D0042: Tip Switch, Value = 0 to 1
	uint8_t  Finger0_InRange : 1;                       // Usage 0x000D0032: In Range, Value = 0 to 1
//	uint8_t data;
	uint8_t  Finger0_ContactIdentifier;                 // Usage 0x000D0051: Contact Identifier, Value = 0 to 5
	uint16_t Finger0_X;                                 // Usage 0x00010030: X, Value = 0 to 32767
	uint16_t Finger0_Y;                                 // Usage 0x00010031: Y, Value = 0 to 32767
	// Collection: TouchScreen Finger(1)
	uint8_t  Finger1_TipSwitch : 1;                     // Usage 0x000D0042: Tip Switch, Value = 0 to 1
	uint8_t  Finger1_InRange : 1;                       // Usage 0x000D0032: In Range, Value = 0 to 1
	uint8_t  Finger1_ContactIdentifier;                 // Usage 0x000D0051: Contact Identifier, Value = 0 to 5
	uint16_t Finger1_X;                                 // Usage 0x00010030: X, Value = 0 to 32767
	uint16_t Finger1_Y;                                 // Usage 0x00010031: Y, Value = 0 to 32767
	// Collection: TouchScreen Finger(2)
	uint8_t  Finger2_TipSwitch : 1;                     // Usage 0x000D0042: Tip Switch, Value = 0 to 1
	uint8_t  Finger2_InRange : 1;                       // Usage 0x000D0032: In Range, Value = 0 to 1
	uint8_t  Finger2_ContactIdentifier;                 // Usage 0x000D0051: Contact Identifier, Value = 0 to 5
	uint16_t Finger2_X;                                 // Usage 0x00010030: X, Value = 0 to 32767
	uint16_t Finger2_Y;                                 // Usage 0x00010031: Y, Value = 0 to 32767
	// Collection: TouchScreen Finger(3)
	uint8_t  Finger3_TipSwitch : 1;                     // Usage 0x000D0042: Tip Switch, Value = 0 to 1
	uint8_t  Finger3_InRange : 1;                       // Usage 0x000D0032: In Range, Value = 0 to 1
	uint8_t  Finger3_ContactIdentifier;                 // Usage 0x000D0051: Contact Identifier, Value = 0 to 5
	uint16_t Finger3_X;                                 // Usage 0x00010030: X, Value = 0 to 32767
	uint16_t Finger3_Y;                                 // Usage 0x00010031: Y, Value = 0 to 32767
	// Collection: TouchScreen Finger(4)
	uint8_t  Finger4_TipSwitch : 1;                     // Usage 0x000D0042: Tip Switch, Value = 0 to 1
	uint8_t  Finger4_InRange : 1;                       // Usage 0x000D0032: In Range, Value = 0 to 1
	uint8_t  Finger4_ContactIdentifier;                 // Usage 0x000D0051: Contact Identifier, Value = 0 to 5
	uint16_t Finger4_X;                                 // Usage 0x00010030: X, Value = 0 to 32767
	uint16_t Finger4_Y;                                 // Usage 0x00010031: Y, Value = 0 to 32767
	// Collection: TouchScreen
	uint8_t  ContactCount;                              // Usage 0x000D0054: Contact Count, Value = 0 to 8
} USB_DigitizerReport_Data_t;

typedef struct
{
	uint8_t ReportId;
	USB_DigitizerReport_Data_t Data;

} USB_DigitizerReport_t;


void Enp2BulkIn(void);
void USBDeviceCfg(void);
void USBDeviceIntCfg(void);
void USBDeviceEndPointCfg(void);
void DeviceInterrupt(void) __interrupt (INT_NO_USB);
void USB_setHIDCreateHandler(bool (* __xdata handler)(USB_DigitizerReport_Data_t*));


#endif  // __USB_DEF__
