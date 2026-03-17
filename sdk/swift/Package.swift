// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "GraftKit",
    platforms: [.iOS(.v15), .macOS(.v12)],
    products: [
        .library(name: "GraftKit", targets: ["GraftKit"]),
    ],
    targets: [
        .target(name: "GraftKit", path: "Sources/GraftKit"),
        .testTarget(name: "GraftKitTests", dependencies: ["GraftKit"], path: "Tests"),
    ]
)
