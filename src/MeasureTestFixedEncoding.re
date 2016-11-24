/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */

/**
 * Copyright (c) 2014-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
let floatMult = ( *. );

let floatDiv = (/.);

let floatSub = (-.);


/**
 * Since Core_bench is such a huge dependency and it doesn't compile with byte,
 * we include a fake shim of it that we enable by default. To use the far
 * superior Core_bench.
 *
 * - Comment out `include FakeCore;` below.
 * - Uncomment `open Core_bench.Std;`
 * - Delete the two targets in package.json (byteTarget, jsTarget)
 * - Run `npm run build`, then `npm run bench`
 *
 */
let module Node = {
  type context = ref int;
  /* Ignored - only needed to create the dummy instance. */
  let nullContext = {contents: 0};
};

let module Encoding = FixedEncoding;

let module LayoutTypes = LayoutTypes.Create Node Encoding;

let module LayoutSupport = LayoutSupport.Create Node Encoding;

let module Layout = Layout.Create Node Encoding;

let module LayoutTestUtils = LayoutTestUtils.Create Node Encoding;

open LayoutTestUtils;

it
  "should measure correctly"
  (
    fun () => {
      open Encoding;
      let measure node width widthMode height heightMode => {
        node.LayoutTypes.context.contents = node.context.contents + 1;
        {
          LayoutTypes.width: widthMode === LayoutTypes.CssMeasureModeUndefined ? 10 : width,
          height: heightMode === LayoutTypes.CssMeasureModeUndefined ? 10 : height
        }
      };
      let root_style = {...LayoutSupport.defaultStyle, width: 10000, height: 10000};
      let root_child0_style = {...LayoutSupport.defaultStyle, positionType: CssPositionRelative};
      let childContext = {contents: 0};
      let root_child0 =
        LayoutSupport.createNode
          withChildren::[||] andStyle::root_child0_style andMeasure::measure childContext;
      let rootContext = {contents: 0};
      let root =
        LayoutSupport.createNode
          withChildren::[|root_child0|] andStyle::root_style andMeasure::measure rootContext;
      Layout.layoutNode root cssUndefined cssUndefined CssDirectionLtr;
      LayoutTestUtils.assertEq 0 "parent-measure-calls" 0 rootContext.contents;
      LayoutTestUtils.assertEq 1 "parent-measure-calls" 1 childContext.contents
    }
  );

LayoutTestUtils.displayOutcomes ();