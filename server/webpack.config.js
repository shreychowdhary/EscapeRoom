const path = require('path');
const WebpackShellPluginNext = require('webpack-shell-plugin-next');
const nodeExternals = require('webpack-node-externals');

const { NODE_ENV = 'production' } = process.env;

module.exports = {
    entry: './src/server.ts',
    externals: [nodeExternals()],
    mode: NODE_ENV,
    watch: NODE_ENV === 'development',
    target: 'node',
    node: {
        __dirname: false,
        __filename: false
    },
    output: {
        path: path.resolve(__dirname, 'build'),
        filename: 'index.js'
    },
    resolve: {
        extensions: ['.ts', '.js']
    },
    module: {
        rules: [
            {
                test: /\.ts$/,
                use: ['ts-loader']
            }
        ]
    },
    plugins: NODE_ENV === 'development'
        ? [
            new WebpackShellPluginNext({
                onBuildEnd: {
                    scripts: ['yarn run:dev'],
                },
            })
        ] : [
            new WebpackShellPluginNext({
                onBuildEnd: {
                    scripts: ['yarn run:prod'],
                },
            })
        ]
};