/*
 Copyright (C) 2019-2022 Fredrik Öhrström (gpl-3.0-or-later)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include"meters_common_implementation.h"

namespace
{
    struct Driver : public virtual MeterCommonImplementation
    {
        Driver(MeterInfo &mi, DriverInfo &di);
    };

    static bool ok = registerDriver([](DriverInfo&di)
    {
        di.setName("eurisii");
        di.setDefaultFields("name,id,current_consumption_hca,status,timestamp");
        di.setMeterType(MeterType::HeatCostAllocationMeter);
        di.addLinkMode(LinkMode::T1);
        di.addDetection(MANUFACTURER_INE,  0x08,  0x55);
        di.setConstructor([](MeterInfo& mi, DriverInfo& di){ return shared_ptr<Meter>(new Driver(mi, di)); });
    });

    Driver::Driver(MeterInfo &mi, DriverInfo &di) : MeterCommonImplementation(mi, di)
    {
        addStringFieldWithExtractorAndLookup(
            "status",
            "Meter status from error flags and tpl status field.",
            PrintProperty::JSON | PrintProperty::IMPORTANT |
            PrintProperty::STATUS | PrintProperty::JOIN_TPL_STATUS,
            FieldMatcher::build()
            .set(MeasurementType::Instantaneous)
            .set(VIFRange::ErrorFlags),
            Translate::Lookup(
            {
                {
                    {
                        "ERROR_FLAGS",
                        Translate::Type::BitToString,
                        0xffff,
                        "OK",
                        {
                            { 0x0001, "MEASUREMENT" },
                            { 0x0002, "SABOTAGE" },
                            { 0x0004, "BATTERY" },
                            { 0x0008, "CS" },
                            { 0x0010, "HF" },
                            { 0x0020, "RESET" }
                        }
                    },
                },
            }));

        addStringFieldWithExtractorAndLookup(
            "error_flags",
            "Deprecated field! Use status instead.",
            PrintProperty::JSON | PrintProperty::DEPRECATED |
            PrintProperty::STATUS | PrintProperty::JOIN_TPL_STATUS,
            FieldMatcher::build()
            .set(MeasurementType::Instantaneous)
            .set(VIFRange::ErrorFlags),
            Translate::Lookup(
            {
                {
                    {
                        "ERROR_FLAGS",
                        Translate::Type::BitToString,
                        0xffff,
                        "OK",
                        {
                            { 0x0001, "MEASUREMENT" },
                            { 0x0002, "SABOTAGE" },
                            { 0x0004, "BATTERY" },
                            { 0x0008, "CS" },
                            { 0x0010, "HF" },
                            { 0x0020, "RESET" }
                        }
                    },
                },
            }));

        addNumericFieldWithExtractor(
            "current_consumption",
            "The current heat cost allocation.",
            PrintProperty::JSON,
            Quantity::HCA,
            VifScaling::Auto,
            FieldMatcher::build()
            .set(MeasurementType::Instantaneous)
            .set(VIFRange::HeatCostAllocation)
            );

        addNumericFieldWithExtractor(
            "consumption_at_set_date_{storage_counter}",
            "The heat cost allocation at set date #.",
            PrintProperty::JSON,
            Quantity::HCA,
            VifScaling::Auto,
            FieldMatcher::build()
            .set(MeasurementType::Instantaneous)
            .set(VIFRange::HeatCostAllocation)
            .set(StorageNr(1),StorageNr(17))
            );

        addNumericFieldWithExtractor(
            "consumption_at_set_date",
            "Deprecated field.",
            PrintProperty::JSON,
            Quantity::HCA,
            VifScaling::Auto,
            FieldMatcher::build()
            .set(MeasurementType::Instantaneous)
            .set(VIFRange::HeatCostAllocation)
            .set(StorageNr(1))
            );


    }
}

// Test: HeatMeter eurisii 88018801 NOKEY
// telegram=|7644C52501880188550872_01880188C5255508010000002F2F0B6E332211426E110182016E1102C2016E110382026E1104C2026E110582036E1106C2036E110782046E1108C2046E110982056E1110C2056E111182066E1112C2066E111382076E1114C2076E111582086E1116C2086E111702FD172100|
// {"media":"heat cost allocation","meter":"eurisii","name":"HeatMeter","id":"88018801","status":"MEASUREMENT RESET","current_consumption_hca":112233,"consumption_at_set_date_hca":273,"consumption_at_set_date_1_hca":273,"consumption_at_set_date_2_hca":529,"consumption_at_set_date_3_hca":785,"consumption_at_set_date_4_hca":1041,"consumption_at_set_date_5_hca":1297,"consumption_at_set_date_6_hca":1553,"consumption_at_set_date_7_hca":1809,"consumption_at_set_date_8_hca":2065,"consumption_at_set_date_9_hca":2321,"consumption_at_set_date_10_hca":4113,"consumption_at_set_date_11_hca":4369,"consumption_at_set_date_12_hca":4625,"consumption_at_set_date_13_hca":4881,"consumption_at_set_date_14_hca":5137,"consumption_at_set_date_15_hca":5393,"consumption_at_set_date_16_hca":5649,"consumption_at_set_date_17_hca":5905,"error_flags":"MEASUREMENT RESET","timestamp":"1111-11-11T11:11:11Z"}
// |HeatMeter;88018801;112233;MEASUREMENT RESET;1111-11-11 11:11.11
